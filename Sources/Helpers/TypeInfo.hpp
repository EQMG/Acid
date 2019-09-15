#pragma once

#include "StdAfx.hpp"

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
		static const auto id = NextTypeId();
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
};

template<typename K>
TypeId TypeInfo<K>::m_nextTypeId = 0;
}
