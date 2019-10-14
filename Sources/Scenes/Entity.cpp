#include "Entity.inl"

#include "Scene.hpp"

namespace acid {
Entity::Entity(Id id, Scene &scene) :
	m_id(id),
	m_scene(scene) {
}

void Entity::RemoveAllComponents() {
	m_scene.value()->m_components.RemoveAllComponents(m_id);
	m_scene.value()->RefreshEntity(m_id);
}

std::string Entity::GetName() const {
	return m_scene.value()->GetEntityName(m_id);
}

bool Entity::IsEnabled() const {
	return m_scene.value()->IsEntityEnabled(m_id);
}

void Entity::Enable() {
	m_scene.value()->EnableEntity(m_id);
}

void Entity::Disable() {
	m_scene.value()->DisableEntity(m_id);
}

bool Entity::IsValid() const {
	return m_scene.has_value() && m_scene.value()->IsEntityValid(m_id);
}

void Entity::Remove() {
	m_scene.value()->RemoveEntity(m_id);
}

bool Entity::operator==(const Entity &other) const {
	return m_id == other.m_id && m_scene == other.m_scene;
}

bool Entity::operator!=(const Entity &other) const {
	return !(*this == other);
}
}
