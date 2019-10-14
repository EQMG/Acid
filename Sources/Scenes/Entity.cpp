#include "Entity.inl"

#include "Scene.hpp"

namespace acid {
Entity::Entity(Id id, Scene *scene) :
	m_id(id),
	m_scene(scene) {
}

void Entity::RemoveAllComponents() {
	m_scene->m_components.RemoveAllComponents(m_id);
	m_scene->RefreshEntity(m_id);
}

std::string Entity::GetName() const {
	return m_scene->GetEntityName(m_id);
}

bool Entity::IsEnabled() const {
	return m_scene->IsEntityEnabled(m_id);
}

void Entity::Enable() {
	m_scene->EnableEntity(m_id);
}

void Entity::Disable() {
	m_scene->DisableEntity(m_id);
}

bool Entity::IsValid() const {
	return m_scene && m_scene->IsEntityValid(m_id);
}

void Entity::Remove() {
	m_scene->RemoveEntity(m_id);
}

bool Entity::operator==(const Entity &other) const {
	return m_id == other.m_id && m_scene == other.m_scene;
}

bool Entity::operator!=(const Entity &other) const {
	return !(*this == other);
}
}
