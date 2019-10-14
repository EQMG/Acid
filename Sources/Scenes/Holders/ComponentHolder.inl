#pragma once

#include "ComponentHolder.hpp"

namespace acid {

template<typename T>
bool ComponentHolder::HasComponent(Entity::Id id) const {
	// Is the Entity ID and the Component type ID known.
	if (id < m_components.size()) {
		auto typeId = GetComponentTypeId<T>();

		// Is the Component type ID known
		if (typeId < m_components[id].size()) {
			return m_components[id][typeId] != nullptr;
		}
	}

	return false;
}

template<typename T>
T *ComponentHolder::GetComponent(Entity::Id id) const {
	if (!HasComponent<T>(id)) {
		//throw std::runtime_error("Entity does not have requested Component");
		return nullptr;
	}

	auto &component = m_components[id][GetComponentTypeId<T>()];

	if (!component.get()) {
		//throw std::runtime_error("Entity does not have requested Component");
		return nullptr;
	}

	return static_cast<T *>(component.get());
}

template<typename T>
void ComponentHolder::AddComponent(Entity::Id id, std::unique_ptr<T> &&component) {
	if (id >= m_components.size()) {
		throw std::runtime_error("Entity ID is out of range");
	}

	const auto typeId = GetComponentTypeId<T>();
	if (const auto componentTypeId = component->GetTypeId(); typeId != componentTypeId) {
		Log::Error("NO");
	}

	if (typeId >= m_components[id].size()) {
		throw std::runtime_error("Component type ID is out of range");
	}

	m_components[id][typeId] = std::move(component);
	m_componentsMasks[id].set(typeId);
}

template<typename T>
void ComponentHolder::RemoveComponent(Entity::Id id) {
	if (!HasComponent<T>(id)) {
		return;
	}

	auto &component = m_components[id][GetComponentTypeId<T>()];
	component.reset();
	m_componentsMasks[id].reset(GetComponentTypeId<T>());
}
}
