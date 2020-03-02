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
		if (auto it = typeMap.find(typeIndex); it != typeMap.end())
			return it->second;
		const auto id = NextTypeId();
		typeMap[typeIndex] = id;
		return id;
	}

private:
	/**
	 * Get the next type ID for T
	 * @return The next type ID for T.
	 */
	static TypeId NextTypeId() noexcept {
		const auto id = nextTypeId;
		++nextTypeId;
		return id;
	}

	// Next type ID for T.
	inline static TypeId nextTypeId = 0;
	inline static std::unordered_map<std::type_index, TypeId> typeMap = {};
};
}
