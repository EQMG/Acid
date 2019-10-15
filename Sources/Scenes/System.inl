#pragma once

#include "System.hpp"

namespace acid {
template<typename Func>
void System::ForEach(Func &&func) {
	for (const auto &entity : m_enabledEntities) {
		if (entity.IsValid()) {
			func(entity);
		}
	}
}

template<typename ...Ts, typename Func>
void System::ForJoinedEach(Func &&func) {
	for (const auto &entity : m_enabledEntities) {
		if (entity.IsValid()) {
			func(entity, ((entity.GetComponent<Ts>()), ...));
		}
	}
}

template class ACID_EXPORT TypeInfo<System>;

template<typename T>
TypeId GetSystemTypeId() noexcept {
	static_assert(std::is_base_of<System, T>::value, "T must be a System.");

	return TypeInfo<System>::GetTypeId<T>();
}
}
