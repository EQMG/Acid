#pragma once

#include <typeindex>
#include <unordered_map>

namespace acid {
using TypeId = std::size_t;

template<typename T>
class TypeInfo {
public:
	TypeInfo() = delete;

	/**
	 * Get the type ID of K which is a base of T.
	 * @tparam K The type ID K.
	 * @return The type ID.
	 */
	template<typename K>
	static TypeId GetTypeId() noexcept {
		auto typeIndex = std::type_index(typeid(K));
		if (auto it = m_typeMap.find(typeIndex); it != m_typeMap.end())
			return it->second;
		const auto id = NextTypeId();
		m_typeMap[typeIndex] = id;
		return id;
	}

private:
	/**
	 * Get the next type ID for T
	 * @return The next type ID for T.
	 */
	static TypeId NextTypeId() noexcept {
		const auto id = m_nextTypeId;
		++m_nextTypeId;
		return id;
	}

	// Next type ID for T.
	static TypeId m_nextTypeId;
	static std::unordered_map<std::type_index, TypeId> m_typeMap;
};

template<typename K>
TypeId TypeInfo<K>::m_nextTypeId = 0;

template<typename K>
std::unordered_map<std::type_index, TypeId> TypeInfo<K>::m_typeMap = {};
}
