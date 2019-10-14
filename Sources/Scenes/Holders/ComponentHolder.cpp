#include "ComponentHolder.hpp"

namespace acid {
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
