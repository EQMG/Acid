#pragma once

#include "Files/Node.hpp"

// https://dalzhim.github.io/2017/08/11/Improving-the-enum-class-bitmask
// https://stackoverflow.com/a/8498694/6251899

namespace acid {
template<typename Enum>
struct EnableBitMaskOperators :
	std::false_type {
};

#define ENABLE_BITMASK_OPERATORS(T) \
    template<>                      \
    struct EnableBitMaskOperators<T> : std::true_type {};

template<typename Enum>
inline constexpr bool EnableBitMaskOperatorsV = EnableBitMaskOperators<Enum>::value;

/**
 * @brief Wrapper for an enumerator that provides implicit bool conversion
 */
template<typename T>
class Enumerator {
public:
	constexpr Enumerator(const T &value) :
		value(value) {
	}

	constexpr operator bool() const {
		using underlying_type = typename std::underlying_type_t<T>;
		return static_cast<underlying_type>(value) != 0;
	}

	constexpr operator T() const { return value; }

	T value;
};

/**
 * @brief Wrapper that differentiates combined enumerators from a single enumerator
 * to prevent accidental comparisons between a bitmask and a single enumerator
 * using operator== or operator!=
 */
template<typename T>
class BitMask {
public:
	using underlying_type = typename std::underlying_type_t<T>;

	constexpr BitMask() :
		value(static_cast<underlying_type>(0)) {
	}

	constexpr BitMask(const T &value) :
		value(static_cast<underlying_type>(value)) {
	}

	constexpr BitMask(const underlying_type &value) :
		value(value) {
	}

	constexpr BitMask(const Enumerator<T> &enumerator) :
		value(static_cast<underlying_type>(enumerator.value)) {
	}

	friend const Node &operator>>(const Node &node, BitMask &bitmask) {
		node >> bitmask.value;
		return node;
	}

	friend Node &operator<<(Node &node, const BitMask &bitmask) {
		node << bitmask.value;
		return node;
	}

	constexpr operator bool() const { return value != 0; }

	underlying_type value;
};

template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> MakeBitMask(const T &t) {
	return {t};
}

template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> MakeBitMask(const std::underlying_type_t<T> &t) {
	return {t};
}

/**
 * operator&(T, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, Enumerator<T>> operator&(const T &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	assert((static_cast<underlying_type>(lhs) & (static_cast<underlying_type>(lhs) - 1)) == 0);
	return {static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs))};
}

/**
 * operator&(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, Enumerator<T>> operator&(const Enumerator<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & static_cast<underlying_type>(rhs.value))};
}

/**
 * operator&(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator&(const BitMask<T> &lhs, const BitMask<T> &rhs) {
	return {static_cast<T>(lhs.value & rhs.value)};
}

/**
 * operator&(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, Enumerator<T>> operator&(const BitMask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value & static_cast<underlying_type>(rhs))};
}

/**
 * operator&(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, Enumerator<T>> operator&(const T &lhs, const BitMask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) & rhs.value)};
}

/**
 * operator&(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, Enumerator<T>> operator&(const BitMask<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value & static_cast<underlying_type>(rhs.value))};
}

/**
 * operator&(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, Enumerator<T>> operator&(const Enumerator<T> &lhs, const BitMask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & rhs.value)};
}

/**
 * operator&(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, Enumerator<T>> operator&(const T &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs.value))};
}

/**
 * operator&(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, Enumerator<T>> operator&(const Enumerator<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & static_cast<underlying_type>(rhs))};
}

/**
 * operator|(T, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const T &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs))};
}

/**
 * operator|(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const Enumerator<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | static_cast<underlying_type>(rhs.value))};
}

/**
 * operator|(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const BitMask<T> &lhs, const BitMask<T> &rhs) {
	return {static_cast<T>(lhs.value | rhs.value)};
}

/**
 * operator|(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const BitMask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value | static_cast<underlying_type>(rhs))};
}

/**
 * operator|(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const T &lhs, const BitMask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) | rhs.value)};
}

/**
 * operator|(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const BitMask<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value | static_cast<underlying_type>(rhs.value))};
}

/**
 * operator|(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const Enumerator<T> &lhs, const BitMask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | rhs.value)};
}

/**
 * operator|(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const Enumerator<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | static_cast<underlying_type>(rhs))};
}

/**
 * operator|(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator|(const T &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs.value))};
}

/**
 * operator^(T, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const T &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs))};
}

/**
 * operator^(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const Enumerator<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ static_cast<underlying_type>(rhs.value))};
}

/**
 * operator^(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const BitMask<T> &lhs, const BitMask<T> &rhs) {
	return {static_cast<T>(lhs.value ^ rhs.value)};
}

/**
 * operator^(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const BitMask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value ^ static_cast<underlying_type>(rhs))};
}

/**
 * operator^(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const T &lhs, const BitMask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ rhs.value)};
}

/**
 * operator^(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const BitMask<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value ^ static_cast<underlying_type>(rhs.value))};
}

/**
 * operator^(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const Enumerator<T> &lhs, const BitMask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ rhs.value)};
}

/**
 * operator^(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const Enumerator<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ static_cast<underlying_type>(rhs))};
}

/**
 * operator^(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator^(const T &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs.value))};
}

/**
 * operator~(T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator~(const T &value) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(~static_cast<underlying_type>(value))};
}

/**
 * operator~(Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator~(const Enumerator<T> &lhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(~static_cast<underlying_type>(lhs.value))};
}

/**
 * operator~(BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T>> operator~(const BitMask<T> &lhs) {
	return {static_cast<T>(~lhs.value)};
}

/**
 * operator&=(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator&=(BitMask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value &= static_cast<underlying_type>(rhs);
	return lhs;
}

/**
 * operator&=(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator&=(BitMask<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value &= static_cast<underlying_type>(rhs.value);
	return lhs;
}

/**
 * operator&=(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator&=(BitMask<T> &lhs, const BitMask<T> &rhs) {
	lhs.value &= rhs.value;
	return lhs;
}

/**
 * operator|=(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator|=(BitMask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value |= static_cast<underlying_type>(rhs);
	return lhs;
}

/**
 * operator|=(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator|=(BitMask<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value |= static_cast<underlying_type>(rhs.value);
	return lhs;
}

/**
 * operator|=(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator|=(BitMask<T> &lhs, const BitMask<T> &rhs) {
	lhs.value |= rhs.value;
	return lhs;
}

/**
 * operator^=(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator^=(BitMask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value ^= static_cast<underlying_type>(rhs);
	return lhs;
}

/**
 * operator^=(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator^=(BitMask<T> &lhs, const Enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value ^= static_cast<underlying_type>(rhs.value);
	return lhs;
}

/**
 * operator^=(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, BitMask<T> &> operator^=(BitMask<T> &lhs, const BitMask<T> &rhs) {
	lhs.value ^= rhs.value;
	return lhs;
}

/**
 * operator==(T, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const T &lhs, const T &rhs) {
	return lhs == rhs;
}

/**
 * operator==(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const Enumerator<T> &lhs, const Enumerator<T> &rhs) {
	return lhs.value == rhs.value;
}

/**
 * operator==(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const BitMask<T> &lhs, const BitMask<T> &rhs) {
	return lhs.value == rhs.value;
}

/**
 * operator==(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const Enumerator<T> &lhs, const T &rhs) {
	return lhs.value == rhs;
}

/**
 * operator==(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const T &lhs, const Enumerator<T> &rhs) {
	return lhs == rhs.value;
}

/**
 * operator==(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const BitMask<T> &lhs, const T &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const T &lhs, const BitMask<T> &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const BitMask<T> &lhs, const Enumerator<T> &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator==(const Enumerator<T> &lhs, const BitMask<T> &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(T, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const T &lhs, const T &rhs) {
	return lhs != rhs;
}

/**
 * operator!=(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const Enumerator<T> &lhs, const Enumerator<T> &rhs) {
	return lhs.value != rhs.value;
}

/**
 * operator!=(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const BitMask<T> &lhs, const BitMask<T> &rhs) {
	return lhs.value != rhs.value;
}

/**
 * operator!=(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const Enumerator<T> &lhs, const T &rhs) {
	return lhs.value != rhs;
}

/**
 * operator!=(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const T &lhs, const Enumerator<T> &rhs) {
	return lhs != rhs.value;
}

/**
 * operator!=(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const BitMask<T> &lhs, const T &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const T &lhs, const BitMask<T> &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const BitMask<T> &lhs, const Enumerator<T> &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, bool> operator!=(const Enumerator<T> &lhs, const BitMask<T> &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator<<(std::ostream, T)
 */
/*template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, std::ostream &> operator<<(std::ostream &lhs, const T &rhs) {
   return lhs << static_cast<std::underlying_type_t<T>>(rhs);
}*/

/**
 * operator>>(std::istream, T)
 */
/*template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitMaskOperatorsV<T>, std::istream &> operator>>(std::istream &lhs, const T &rhs) {
	return lhs >> static_cast<std::underlying_type_t<T>>(rhs);
}*/

/**
 * operator<<(std::ostream, BitMask<T>)
 */
/*template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> &&EnableBitMaskOperatorsV<T>, std::ostream &> operator<<(std::ostream &lhs, const BitMask<T> &rhs) {
   static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
   return lhs << rhs.value;
}*/

/**
* operator>>(std::ostream, BitMask<T>)
*/
/*template<typename T>
constexpr typename std::enable_if_t<std::is_enum_v<T> &&EnableBitMaskOperatorsV<T>, std::istream &> operator>>(std::istream &lhs, const BitMask<T> &rhs) {
	static_assert(!std::is_same_v<typename BitMask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return lhs >> rhs.value;
}*/

template<typename T>
struct EnumIterator {
	struct Iterator {
		using underlying_type = typename std::underlying_type_t<T>;

		Iterator(const underlying_type &value) :
			m_value(value) {
		}

		T operator*() const { return static_cast<T>(m_value); }
		void operator++() { ++m_value; }
		bool operator!=(Iterator rhs) { return m_value != rhs.m_value; }

		underlying_type m_value;
	};
};

template<typename T>
typename EnumIterator<T>::Iterator begin(EnumIterator<T>) {
	using underlying_type = typename EnumIterator<T>::Iterator::underlying_type;
	return typename EnumIterator<T>::Iterator(static_cast<underlying_type>(T::First));
}

template<typename T>
typename EnumIterator<T>::Iterator end(EnumIterator<T>) {
	using underlying_type = typename std::underlying_type_t<T>;
	return typename EnumIterator<T>::Iterator(static_cast<underlying_type>(T::Last) + 1);
}
}
