#pragma once

#include "Entity.hpp"
#include "Scene.hpp"

namespace acid {
template<typename T>
bool Entity::HasComponent() const {
	return m_scene->m_components.HasComponent<T>(m_id);
}

template<typename T>
T *Entity::GetComponent() const {
	return m_scene->m_components.GetComponent<T>(m_id);
}

template<typename T, typename... Args>
T *Entity::AddComponent(Args &&...args) {
	m_scene->m_components.AddComponent<T>(m_id, std::make_unique<T>(std::forward<Args>(args)...));
	m_scene->RefreshEntity(m_id);
	return GetComponent<T>();
}

template<typename T>
void Entity::RemoveComponent() {
	m_scene->m_components.RemoveComponent<T>(m_id);
	m_scene->RefreshEntity(m_id);
}
}
