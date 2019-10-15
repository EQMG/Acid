#pragma once

#include "ComponentHolder.hpp"

namespace acid {

template<typename T>
std::vector<T *> ComponentHolder::GetComponents() const {
	std::vector<T *> result;
	const auto typeId = GetComponentTypeId<T>();
	for (const auto &components : m_components) {
		if (const auto &component = components[typeId])
			result.emplace_back(static_cast<T *>(component.get()));
	}
	return result;
}

template<typename T>
bool ComponentHolder::HasComponent(Entity::Id id) const {
	// Is the Entity ID and the Component type ID known.
	if (id < m_components.size()) {
		auto typeId = GetComponentTypeId<T>();

		// Is the Component type ID known
		if (typeId < m_components[id].size())
			return m_components[id][typeId] != nullptr;
	}

	return false;
}

template<typename T>
T *ComponentHolder::GetComponent(Entity::Id id) const {
	if (!HasComponent<T>(id)) {
		//throw std::runtime_error("Entity does not have requested Component");
		return nullptr;
	}

	const auto typeId = GetComponentTypeId<T>();
	auto &component = m_components[id][typeId];

	if (!component) {
		//throw std::runtime_error("Entity does not have requested Component");
		return nullptr;
	}

	return static_cast<T *>(component.get());
}

template<typename T>
void ComponentHolder::AddComponent(Entity::Id id, std::unique_ptr<T> &&component) {
	const auto typeId = GetComponentTypeId<T>();

	// If we ever have a disagreement from {@link TypeInfo} throw an exception.
	if (typeId != component->GetTypeId()) {
		throw std::runtime_error("Static Component Type ID does not match the member function");
	}

	AddComponent(id, typeId, std::move(component));
}

template<typename T>
void ComponentHolder::RemoveComponent(Entity::Id id) {
	if (!HasComponent<T>(id))
		return;

	auto &component = m_components[id][GetComponentTypeId<T>()];
	component.reset();
	m_componentsMasks[id].reset(GetComponentTypeId<T>());
}
}
