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

template<typename ...Types>
void System::ForEach(typename std::common_type<std::function<void(Entity, std::add_pointer_t<Types>...)>>::type func) {
	for (const auto &entity : m_enabledEntities) {
		if (entity.IsValid()) {
			func(entity, entity.template GetComponent<Types>()...);
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
