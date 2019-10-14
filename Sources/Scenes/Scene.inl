#pragma once

#include "Scene.hpp"

namespace acid {
template<typename T>
std::vector<T *> Scene::GetComponents() const {
	std::vector<T *> result;
	for (auto entity : m_entities) {
		if (auto component = entity.m_entity.GetComponent<T>())
			result.emplace_back(component);
	}
	return result;
}

template<typename T>
bool Scene::HasSystem() const {
	return m_systems.HasSystem<T>();
}

template<typename T>
T *Scene::GetSystem() const {
	return m_systems.GetSystem<T>();
}

template<typename T, typename... Args>
T *Scene::AddSystem(std::size_t priority, Args &&...args) {
	m_systems.AddSystem<T>(priority, std::make_unique<T>(std::forward<Args>(args)...));

	auto system = GetSystem<T>();
	m_newSystems.emplace_back(system);

	// Sets the System Scene.
	system->m_scene = this;
	return system;
}

template<typename T>
void Scene::RemoveSystem() {
	m_systems.RemoveSystem<T>();
}
}
