#pragma once

#include <bitmask/bitmask.hpp>

#include "Filesystem/Serial/Node.hpp"

namespace acid {
template<typename E, typename = std::enable_if_t<std::is_enum_v<std::remove_reference_t<E>>>>
class EnumValue {
public:
	constexpr EnumValue(const E &value = {}) : value(value) {}
	template<typename E1 = E, typename = std::enable_if_t<!std::is_reference_v<E1>>>
	constexpr EnumValue(E1 &&value) : value(std::move(value)) {}

	friend const Node &operator>>(const Node &node, EnumValue<E> &object) {
		object.value = magic_enum::enum_cast<std::remove_reference_t<E>>(node.Get<std::string>()).value();
		return node;
	}

	friend Node &operator<<(Node &node, const EnumValue<E> &object) {
		return node << magic_enum::enum_name(object.value);
	}

	E operator*() noexcept { return std::move(value); }
	operator E () noexcept { return std::move(value); }

	template<typename K = std::remove_reference_t<E>>
	constexpr EnumValue &operator=(const K &value) noexcept {
		this->value = value;
		return *this;
	}

protected:
	E value;
};

template<typename E>
class EnumValueRef : public EnumValue<E &> {
public:
	constexpr EnumValueRef(E &value) : EnumValue<E &>(value) {}

	operator const E &() const { return this->value; }
	const E &operator*() const { return this->value; }

	template<typename K>
	constexpr EnumValueRef &operator=(const K &value) noexcept {
		this->value = value;
		return *this;
	}
};
}
