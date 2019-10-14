#include "ComponentHolder.hpp"

namespace acid {
std::vector<Component *> ComponentHolder::GetComponents(Entity::Id id) const {
	std::vector<Component *> result;
	for (const auto &component : m_components[id]) {
		if (component)
			result.emplace_back(component.get());
	}
	return result;
}

void ComponentHolder::AddComponent(Entity::Id id, TypeId typeId, std::unique_ptr<Component> &&component) {
	if (id >= m_components.size()) {
		throw std::runtime_error("Entity ID is out of range");
	}

	if (typeId >= m_components[id].size()) {
		throw std::runtime_error("Component type ID is out of range");
	}

	m_components[id][typeId] = std::move(component);
	m_componentsMasks[id].set(typeId);
}

void ComponentHolder::RemoveAllComponents(Entity::Id id) {
	if (id < m_components.size()) {
		for (auto &component : m_components[id]) {
			component.reset();
		}

		m_componentsMasks[id].reset();
	}
}

ComponentFilter::Mask ComponentHolder::GetComponentsMask(Entity::Id id) const {
	if (id < m_componentsMasks.size()) {
		return m_componentsMasks[id];
	}

	return {};
}

void ComponentHolder::Resize(std::size_t size) {
	m_components.resize(size);
	m_componentsMasks.resize(size);
}

void ComponentHolder::Clear() noexcept {
	m_components.clear();
	m_componentsMasks.clear();
}
}
