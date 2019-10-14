#include "Scene.hpp"

#include "Entity.inl"
#include "EntityPrefab.hpp"

namespace acid {
Scene::Scene(std::unique_ptr<Camera> &&camera) :
	m_camera(std::move(camera)) {
}

Scene::~Scene() {
	Clear();
}

void Scene::RemoveAllSystems() {
	m_systems.RemoveAllSystems();
}

Entity Scene::CreateEntity() {
	const auto id = m_pool.Create();

	// Resize containers if necessary.
	Extend(id + 1);

	m_entities[id].m_entity = Entity(id, this);
	m_entities[id].m_enabled = true;
	m_entities[id].m_valid = true;

	EnableEntity(m_entities[id].m_entity);

	return m_entities[id].m_entity;
}

Entity Scene::CreateEntity(const std::string &name) {
	if (m_names.find(name) != m_names.end()) {
		throw std::runtime_error("Entity name already in use");
	}

	const auto entity = CreateEntity();

	m_names[name] = entity.GetId();
	m_entities[entity.GetId()].m_name = name;

	return entity;
}

Entity Scene::CreatePrefabEntity(const std::string &filename) {
	auto entity = CreateEntity();
	auto prefab = EntityPrefab::Create(filename);
	*prefab >> entity;
	return entity;
}

std::optional<Entity> Scene::GetEntity(Entity::Id id) const {
	if (!IsEntityValid(id)) {
		return std::nullopt;
	}

	return m_entities[id].m_entity;
}

std::optional<Entity> Scene::GetEntity(const std::string &name) const {
	const auto it = m_names.find(name);

	if (it == m_names.end()) {
		return std::nullopt;
	}

	return GetEntity(it->second);
}

std::string Scene::GetEntityName(Entity::Id id) const {
	if (!IsEntityValid(id)) {
		throw std::runtime_error("Entity ID is not valid");
	}

	if (m_entities[id].m_name.has_value()) {
		return m_entities[id].m_name.value();
	}

	return {};
}

bool Scene::IsEntityEnabled(Entity::Id id) const {
	return IsEntityValid(id) && m_entities[id].m_enabled;
}

void Scene::EnableEntity(Entity::Id id) {
	if (!IsEntityValid(id)) {
		throw std::runtime_error("Entity ID is not valid");
	}

	m_actions.emplace_back(EntityAction(id, EntityAction::Action::Enable));
}

void Scene::DisableEntity(Entity::Id id) {
	if (!IsEntityValid(id)) {
		throw std::runtime_error("Entity ID is not valid");
	}

	m_actions.emplace_back(EntityAction(id, EntityAction::Action::Disable));
}

bool Scene::IsEntityValid(Entity::Id id) const {
	return id < m_entities.size() && m_entities[id].m_valid;
}

void Scene::RemoveEntity(Entity::Id id) {
	if (!IsEntityValid(id)) {
		throw std::runtime_error("Entity ID is not valid");
	}

	m_actions.emplace_back(EntityAction(id, EntityAction::Action::Remove));
}

void Scene::RefreshEntity(Entity::Id id) {
	if (!IsEntityValid(id)) {
		throw std::runtime_error("Entity ID is not valid");
	}

	m_actions.emplace_back(EntityAction(id, EntityAction::Action::Refresh));
}

void Scene::RemoveAllEntities() {
	for (const auto &entity : m_entities) {
		// We may iterate through invalid entities.
		if (entity.m_valid) {
			RemoveEntity(entity.m_entity.GetId());
		}
	}
}

void Scene::Update(float delta) {
	// Start new Systems
	for (auto &system : m_newSystems) {
		system->OnStart();
	}

	m_newSystems.clear();

	UpdateEntities();
	m_systems.ForEach([delta](System &system, TypeId) {
		system.Update(delta);
	});
}

void Scene::Clear() {
	RemoveAllSystems();

	m_entities.clear();
	m_actions.clear();
	m_names.clear();

	m_components.Clear();
	m_pool.Reset();
}

void Scene::UpdateEntities() {
	// Here, we copy m_actions to make possible to create, enable, etc.
	// Entities within event handlers like system::onEntityAttached, etc.
	const auto actionsList = std::move(m_actions);
	m_actions = decltype(m_actions)();

	for (const auto &action : actionsList) {
		try {
			ExecuteAction(action);
		} catch (const std::exception & e) {
			std::cout << e.what() << '\n';
		}
	}
}

void Scene::ExecuteAction(const EntityAction &action) {
	if (!IsEntityValid(action.id)) {
		throw std::runtime_error("Entity action ID is not valid");
	}

	switch (action.action) {
	case EntityAction::Action::Enable:
		ActionEnable(action.id);
		break;
	case EntityAction::Action::Disable:
		ActionDisable(action.id);
		break;
	case EntityAction::Action::Remove:
		ActionRemove(action.id);
		break;
	case EntityAction::Action::Refresh:
		ActionRefresh(action.id);
		break;
	}
}

void Scene::ActionEnable(Entity::Id id) {
	m_systems.ForEach([&](System &system, TypeId systemId) {
		const auto attachStatus = TryEntityAttach(system, systemId, id);

		if (attachStatus == EntityAttachStatus::AlreadyAttached || attachStatus == EntityAttachStatus::Attached) {
			// The Entity is attached to the System, it is enabled.
			system.EnableEntity(m_entities[id].m_entity);
		}
	});
}

void Scene::ActionDisable(Entity::Id id) {
	m_entities[id].m_enabled = false;

	m_systems.ForEach([&](System &system, TypeId systemId) {
		// Is the Entity attached to the System?
		if (systemId < m_entities[id].m_systems.size() && m_entities[id].m_systems[systemId]) {
			system.DisableEntity(m_entities[id].m_entity);
		}
	});
}

void Scene::ActionRemove(Entity::Id id) {
	m_systems.ForEach([&](System &system, TypeId systemId) {
		// Is the Entity attached to the System?
		if (systemId < m_entities[id].m_systems.size() && m_entities[id].m_systems[systemId]) {
			system.DetachEntity(m_entities[id].m_entity);
			m_entities[id].m_systems[systemId] = false;
		}
	});

	// Invalidate the Entity and reset its attributes.
	m_entities[id].m_valid = false;
	m_entities[id].m_systems.clear();

	// Remove its name from the list
	if (m_entities[id].m_name.has_value()) {
		m_names.erase(m_entities[id].m_name.value());
		m_entities[id].m_name.reset();
	}

	m_components.RemoveAllComponents(id);
	m_pool.Store(id);
}

void Scene::ActionRefresh(Entity::Id id) {
	m_systems.ForEach([&](System &system, TypeId systemId) {
		const auto attachStatus = TryEntityAttach(system, systemId, id);

		if (m_entities[id].m_enabled && attachStatus == EntityAttachStatus::Attached) {
			// If the Entity has been attached and is enabled, enable it into the System.
			system.EnableEntity(m_entities[id].m_entity);
		}
	});
}

void Scene::Extend(std::size_t size) {
	if (size > m_entities.size()) {
		m_entities.resize(size);
		m_components.Resize(size);
	}
}

Scene::EntityAttachStatus Scene::TryEntityAttach(System &system, TypeId systemId, Entity::Id id) {
	// Does the Entity match the requirements to be part of the System?
	if (system.GetFilter().Check(m_components.GetComponentsMask(id))) {
		// Is the Entity not already attached to the System?
		if (systemId >= m_entities[id].m_systems.size() || !m_entities[id].m_systems[systemId]) {
			if (systemId >= m_entities[id].m_systems.size()) {
				m_entities[id].m_systems.resize(systemId + 1, false);
			}

			m_entities[id].m_systems[systemId] = true;
			system.AttachEntity(m_entities[id].m_entity);

			// The Entity has been attached to the System.
			return EntityAttachStatus::Attached;
		}

		// Otherwise, if the Entity is already attached to the System.
		return EntityAttachStatus::AlreadyAttached;
	}

	// If the Entity is already attached to the System but doest not match the requirements anymore, we detach it from the System.
	if (systemId < m_entities[id].m_systems.size() && m_entities[id].m_systems[systemId]) {
		system.DetachEntity(m_entities[id].m_entity);
		m_entities[id].m_systems[systemId] = false;

		// The Entity has been detached from the System.
		return EntityAttachStatus::Detached;
	}

	// Nothing happened because the Entity is not attached to the System and does not match the requirements to be part of it.
	return EntityAttachStatus::NotAttached;
}
}
