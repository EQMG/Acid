#include "System.inl"

#include "Scene.hpp"
#include "System.hpp"

namespace acid {
void System::DetachAll() {
	// Enabled Entities.
	for (auto &entity : m_enabledEntities) {
		OnEntityDisable(entity);
		OnEntityDetach(entity);
	}

	// Disabled Entities.
	for (auto &entity : m_disabledEntities) {
		OnEntityDetach(entity);
	}

	m_enabledEntities.clear();
	m_disabledEntities.clear();

	m_status.clear();
}

void System::AttachEntity(const Entity &entity) {
	if (GetEntityStatus(entity) == EntityStatus::NotAttached) {
		// Add Entity to the Disabled list. The Entity is not enabled by default.
		m_disabledEntities.emplace_back(entity);

		OnEntityAttach(entity);
		SetEntityStatus(entity, EntityStatus::Disabled);
	}
}

void System::DetachEntity(const Entity &entity) {
	const auto status = GetEntityStatus(entity);

	if (status != EntityStatus::NotAttached) {
		if (status == EntityStatus::Enabled) {
			// Remove Entity from Enabled list.
			m_enabledEntities.erase(std::remove(m_enabledEntities.begin(), m_enabledEntities.end(), entity), m_enabledEntities.end());
			OnEntityDisable(entity);
		} else {
			// Remove Entity from Disabled list.
			m_disabledEntities.erase(std::remove(m_disabledEntities.begin(), m_disabledEntities.end(), entity), m_disabledEntities.end());
		}

		OnEntityDetach(entity);
		SetEntityStatus(entity, EntityStatus::NotAttached);
	}
}

void System::EnableEntity(const Entity &entity) {
	if (GetEntityStatus(entity) == EntityStatus::Disabled) {
		// Remove Entity from Disabled list.
		m_disabledEntities.erase(std::remove(m_disabledEntities.begin(), m_disabledEntities.end(), entity), m_disabledEntities.end());

		// Then, add it to the Enabled list.
		m_enabledEntities.emplace_back(entity);

		OnEntityEnable(entity);
		SetEntityStatus(entity, EntityStatus::Enabled);
	}
}

void System::DisableEntity(const Entity &entity) {
	if (GetEntityStatus(entity) == EntityStatus::Enabled) {
		// Remove Entity from Enabled list.
		m_enabledEntities.erase(std::remove(m_enabledEntities.begin(), m_enabledEntities.end(), entity), m_enabledEntities.end());

		// Then, add it to the Disabled list.
		m_disabledEntities.emplace_back(entity);

		OnEntityDisable(entity);
		SetEntityStatus(entity, EntityStatus::Disabled);
	}
}

void System::OnStart() {
}

void System::OnShutdown() {
}

void System::OnEntityAttach(Entity entity) {
}

void System::OnEntityDetach(Entity entity) {
}

void System::OnEntityEnable(Entity entity) {
}

void System::OnEntityDisable(Entity entity) {
}

void System::Update(const Time &delta) {
}

System::EntityStatus System::GetEntityStatus(Entity::Id id) const {
	const auto it = m_status.find(id);

	if (it != m_status.end()) {
		return it->second;
	}

	return EntityStatus::NotAttached;
}

void System::SetEntityStatus(Entity::Id id, const EntityStatus &status) {
	if (status == EntityStatus::NotAttached) {
		m_status.erase(id);
	} else {
		m_status[id] = status;
	}
}
}
