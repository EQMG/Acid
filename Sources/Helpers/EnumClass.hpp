#pragma once

#include "Serialized/Metadata.hpp"

// https://dalzhim.github.io/2017/08/11/Improving-the-enum-class-bitmask
// https://stackoverflow.com/a/8498694/6251899

namespace acid
{
template<typename Enum>
struct EnableBitMaskOperators :
	std::false_type
{
};

#define ENABLE_BITMASK_OPERATORS(T) \
    template<>                      \
    struct EnableBitMaskOperators<T> : std::true_type {};

/**
 * @brief Wrapper for an enumerator that provides implicit bool conversion
 */
template<typename T>
struct Enumerator
{
	constexpr Enumerator(const T &value) :
		m_value{value}
	{
	}

	constexpr operator bool() const
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return static_cast<underlying_type>(m_value) != 0;
	}

	constexpr operator T() const
	{
		return m_value;
	}

	T m_value;
};

/**
 * @brief Wrapper that differentiates combined enumerators from a single enumerator
 * to prevent accidental comparisons between a bitmask and a single enumerator
 * using operator== or operator!=
 */
template<typename T>
struct BitMask
{
	using underlying_type = typename std::underlying_type<T>::type;

	constexpr BitMask() :
		m_value{static_cast<underlying_type>(0)}
	{
	}

	constexpr BitMask(const T &value) :
		m_value{static_cast<underlying_type>(value)}
	{
	}

	constexpr BitMask(const underlying_type &value) :
		m_value{value}
	{
	}

	constexpr BitMask(const Enumerator<T> &enumerator) :
		m_value{static_cast<underlying_type>(enumerator.m_value)}
	{
	}

	friend const Metadata &operator>>(const Metadata &metadata, BitMask &bitmask)
	{
		metadata >> bitmask.m_value;
		return metadata;
	}

	friend Metadata &operator<<(Metadata &metadata, const BitMask &bitmask)
	{
		metadata << bitmask.m_value;
		return metadata;
	}

	constexpr operator bool() const
	{
		return m_value != 0;
	}

	underlying_type m_value;
};

template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type MakeBitMask(const T &t)
{
	return BitMask<T>{ t };
}

template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type MakeBitMask(const typename std::underlying_type<T>::type &t)
{
	return BitMask<T>{ t };
}

/**
 * operator&(T, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const T &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	assert((static_cast<underlying_type>(lhs) & (static_cast<underlying_type>(lhs) - 1)) == 0);
	return Enumerator<T>{ static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs)) };
}

/**
 * operator&(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return Enumerator<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) & static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator&(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator&(const BitMask<T> &lhs, const BitMask<T> &rhs)
{
	return BitMask<T>{ static_cast<T>(lhs.m_value & rhs.m_value) };
}

/**
 * operator&(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const BitMask<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return Enumerator<T>{ static_cast<T>(lhs.m_value & static_cast<underlying_type>(rhs)) };
}

/**
 * operator&(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const T &lhs, const BitMask<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return Enumerator<T>{ static_cast<T>(static_cast<underlying_type>(lhs) & rhs.m_value) };
}

/**
 * operator&(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const BitMask<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return Enumerator<T>{ static_cast<T>(lhs.m_value & static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator&(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const Enumerator<T> &lhs, const BitMask<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return Enumerator<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) & rhs.m_value) };
}

/**
 * operator&(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const T &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return Enumerator<T>{ static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator&(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const Enumerator<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return Enumerator<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) & static_cast<underlying_type>(rhs)) };
}

/**
 * operator|(T, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const T &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs)) };
}

/**
 * operator|(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) | static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator|(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const BitMask<T> &lhs, const BitMask<T> &rhs)
{
	return BitMask<T>{ static_cast<T>(lhs.m_value | rhs.m_value) };
}

/**
 * operator|(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const BitMask<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(lhs.m_value | static_cast<underlying_type>(rhs)) };
}

/**
 * operator|(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const T &lhs, const BitMask<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs) | rhs.m_value) };
}

/**
 * operator|(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const BitMask<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(lhs.m_value | static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator|(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const Enumerator<T> &lhs, const BitMask<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) | rhs.m_value) };
}

/**
 * operator|(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const Enumerator<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) | static_cast<underlying_type>(rhs)) };
}

/**
 * operator|(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator|(const T &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator^(T, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const T &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs)) };
}

/**
 * operator^(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) ^ static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator^(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const BitMask<T> &lhs, const BitMask<T> &rhs)
{
	return BitMask<T>{ static_cast<T>(lhs.m_value ^ rhs.m_value) };
}

/**
 * operator^(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const BitMask<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(lhs.m_value ^ static_cast<underlying_type>(rhs)) };
}

/**
 * operator^(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const T &lhs, const BitMask<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs) ^ rhs.m_value) };
}

/**
 * operator^(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const BitMask<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(lhs.m_value ^ static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator^(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const Enumerator<T> &lhs, const BitMask<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) ^ rhs.m_value) };
}

/**
 * operator^(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const Enumerator<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs.m_value) ^ static_cast<underlying_type>(rhs)) };
}

/**
 * operator^(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator^(const T &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs.m_value)) };
}

/**
 * operator~(T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator~(const T &value)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(~static_cast<underlying_type>(value)) };
}

/**
 * operator~(Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator~(const Enumerator<T> &lhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return BitMask<T>{ static_cast<T>(~static_cast<underlying_type>(lhs.m_value)) };
}

/**
 * operator~(BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T>>::type operator~(const BitMask<T> &lhs)
{
	return BitMask<T>{ static_cast<T>(~lhs.m_value) };
}

/**
 * operator&=(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator&=(BitMask<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	lhs.m_value &= static_cast<underlying_type>(rhs);
	return lhs;
}

/**
 * operator&=(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator&=(BitMask<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	lhs.m_value &= static_cast<underlying_type>(rhs.m_value);
	return lhs;
}

/**
 * operator&=(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator&=(BitMask<T> &lhs, const BitMask<T> &rhs)
{
	lhs.m_value &= rhs.m_value;
	return lhs;
}

/**
 * operator|=(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator|=(BitMask<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	lhs.m_value |= static_cast<underlying_type>(rhs);
	return lhs;
}

/**
 * operator|=(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator|=(BitMask<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	lhs.m_value |= static_cast<underlying_type>(rhs.m_value);
	return lhs;
}

/**
 * operator|=(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator|=(BitMask<T> &lhs, const BitMask<T> &rhs)
{
	lhs.m_value |= rhs.m_value;
	return lhs;
}

/**
 * operator^=(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator^=(BitMask<T> &lhs, const T &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	lhs.m_value ^= static_cast<underlying_type>(rhs);
	return lhs;
}

/**
 * operator^=(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator^=(BitMask<T> &lhs, const Enumerator<T> &rhs)
{
	using underlying_type = typename std::underlying_type<T>::type;
	lhs.m_value ^= static_cast<underlying_type>(rhs.m_value);
	return lhs;
}

/**
 * operator^=(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, BitMask<T> &>::type operator^=(BitMask<T> &lhs, const BitMask<T> &rhs)
{
	lhs.m_value ^= rhs.m_value;
	return lhs;
}

/**
 * operator==(T, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const T &lhs, const T &rhs)
{
	return lhs == rhs;
}

/**
 * operator==(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
{
	return lhs.m_value == rhs.m_value;
}

/**
 * operator==(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const BitMask<T> &lhs, const BitMask<T> &rhs)
{
	return lhs.m_value == rhs.m_value;
}

/**
 * operator==(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Enumerator<T> &lhs, const T &rhs)
{
	return lhs.m_value == rhs;
}

/**
 * operator==(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const T &lhs, const Enumerator<T> &rhs)
{
	return lhs == rhs.m_value;
}

/**
 * operator==(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const BitMask<T> &lhs, const T &rhs)
{
	static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const T &lhs, const BitMask<T> &rhs)
{
	static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const BitMask<T> &lhs, const Enumerator<T> &rhs)
{
	static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator==(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Enumerator<T> &lhs, const BitMask<T> &rhs)
{
	static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(T, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const T &lhs, const T &rhs)
{
	return lhs != rhs;
}

/**
 * operator!=(Enumerator<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
{
	return lhs.m_value != rhs.m_value;
}

/**
 * operator!=(BitMask<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const BitMask<T> &lhs, const BitMask<T> &rhs)
{
	return lhs.m_value != rhs.m_value;
}

/**
 * operator!=(Enumerator<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Enumerator<T> &lhs, const T &rhs)
{
	return lhs.m_value != rhs;
}

/**
 * operator!=(T, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const T &lhs, const Enumerator<T> &rhs)
{
	return lhs != rhs.m_value;
}

/**
 * operator!=(BitMask<T>, T)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const BitMask<T> &lhs, const T &rhs)
{
	static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(T, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const T &lhs, const BitMask<T> &rhs)
{
	static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(BitMask<T>, Enumerator<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const BitMask<T> &lhs, const Enumerator<T> &rhs)
{
	static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

/**
 * operator!=(Enumerator<T>, BitMask<T>)
 */
template<typename T>
constexpr typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Enumerator<T> &lhs, const BitMask<T> &rhs)
{
	static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T>
struct EnumIterator
{
	struct Iterator
	{
		using underlying_type = typename std::underlying_type<T>::type;

		Iterator(const underlying_type &value) :
			m_value(value)
		{
		}

		T operator*() const
		{
			return static_cast<T>(m_value);
		}

		void operator++()
		{
			++m_value;
		}

		bool operator!=(Iterator rhs)
		{
			return m_value != rhs.m_value;
		}

		underlying_type m_value;
	};
};

template<typename T>
typename EnumIterator<T>::Iterator begin(EnumIterator<T>)
{
	using underlying_type = typename EnumIterator<T>::Iterator::underlying_type;
	return typename EnumIterator<T>::Iterator(static_cast<underlying_type>(T::First));
}

template<typename T>
typename EnumIterator<T>::Iterator end(EnumIterator<T>)
{
	using underlying_type = typename std::underlying_type<T>::type;
	return typename EnumIterator<T>::Iterator(static_cast<underlying_type>(T::Last) + 1);
}
}
