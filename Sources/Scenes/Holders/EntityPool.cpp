#include "EntityPool.hpp"

namespace acid {
Entity::Id EntityPool::Create() {
	Entity::Id id;

	if (m_storedIds.empty()) {
		id = m_nextId;
		++m_nextId;
	} else {
		id = m_storedIds.back();
		m_storedIds.pop_back();
	}

	return id;
}

void EntityPool::Store(Entity::Id id) {
	if (id < m_nextId) {
		// Cannot store an ID that haven't been generated before.
		m_storedIds.emplace_back(id);
	}
}

void EntityPool::Reset() noexcept {
	m_storedIds.clear();
	m_nextId = 0;
}
}
