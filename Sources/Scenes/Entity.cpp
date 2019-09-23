#include "Entity.hpp"

#include "Scenes.hpp"
#include "EntityPrefab.hpp"

namespace acid {
Entity::Entity(const std::filesystem::path &filename) {
	auto entityPrefab = EntityPrefab::Create(filename);
	*entityPrefab >> *this;
}

void Entity::Update() {
	for (auto it = m_components.begin(); it != m_components.end();) {
		if ((*it)->IsRemoved()) {
			it = m_components.erase(it);
			continue;
		}

		if ((*it)->GetEntity() != this) {
			(*it)->SetEntity(this);
		}

		if ((*it)->IsEnabled()) {
			if (!(*it)->m_started) {
				(*it)->Start();
				(*it)->m_started = true;
			}

			(*it)->Update();
		}

		++it;
	}
}

Component *Entity::AddComponent(std::unique_ptr<Component> &&component) {
	if (!component) {
		return nullptr;
	}

	component->SetEntity(this);
	return m_components.emplace_back(std::move(component)).get();
}

void Entity::RemoveComponent(Component *component) {
	m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [component](std::unique_ptr<Component> &c) {
		return c.get() == component;
	}), m_components.end());
}

void Entity::RemoveComponent(const std::string &name) {
	m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [&](std::unique_ptr<Component> &c) {
		return name == Component::FindName(c.get());
	}), m_components.end());
}
}
