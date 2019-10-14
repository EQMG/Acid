#pragma once

#include "Helpers/TypeInfo.hpp"
#include "Helpers/StreamFactory.hpp"

namespace acid {
// The maximum number of Components an Entity can holds.
constexpr std::size_t MAX_COMPONENTS = 64;

class Entity;
class ACID_EXPORT Component : public StreamFactory<Component> {
public:
	virtual void Start() {}
	virtual void Update() {}
	Entity *GetEntity() const { return m_entity; }

	Entity *m_entity = nullptr;
	bool m_started = false;
};

/**
 * Gets the Type ID for the Component.
 * @tparam T The Component type.
 * @return The Type ID.
 */
template<typename T>
TypeId GetComponentTypeId() noexcept {
	static_assert(std::is_base_of<Component, T>::value, "T must be a Component.");

	return TypeInfo<Component>::GetTypeId<T>();
}
}
