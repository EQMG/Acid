// https://dalzhim.github.io/2017/08/11/Improving-the-enum-class-bitmask
// https://stackoverflow.com/a/8498694/6251899

#pragma once

#include <cassert>
#include <type_traits>
#include "Serialized/Metadata.hpp"

namespace acid
{
	template<typename Enum>
	struct EnableBitMaskOperators
	{
		static const bool value = false;
	};

#define ENABLE_BITMASK_OPERATORS(T)  \
template<>                           \
struct EnableBitMaskOperators<T>     \
{                                    \
    static constexpr bool value = true; \
};

	/**
	 * Wrapper for an enumerator that provides implicit bool conversion
	 */
	template<typename T>
	struct Enumerator
	{
		constexpr Enumerator(const T &value) :
			m_value(value)
		{
		}

		constexpr explicit operator bool() const
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
	 * Wrapper that differentiates combined enumerators from a single enumerator
	 * to prevent accidental comparisons between a bitmask and a single enumerator
	 * using operator== or operator!=
	 */
	template<typename T>
	struct Bitmask
	{
		using underlying_type = typename std::underlying_type<T>::type;

		constexpr Bitmask() :
			m_value(static_cast<underlying_type>(0))
		{
		}

		constexpr Bitmask(const T &value) :
			m_value(static_cast<underlying_type>(value))
		{
		}

		constexpr Bitmask(const underlying_type &value) :
			m_value(value)
		{
		}

		constexpr Bitmask(const Enumerator<T> &enumerator) :
			m_value(static_cast<underlying_type>(enumerator.value))
		{
		}
		
		void Decode(const Metadata &metadata)
		{
			m_value = metadata.Get<underlying_type>();
		}

		void Encode(Metadata &metadata) const
		{
			metadata.Set(m_value);
		}

		constexpr explicit operator bool() const
		{
			return m_value != 0;
		}

		underlying_type m_value;
	};

	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type MakeBitmask(const T &t)
	{
		return Bitmask<T>{t};
	}

	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type MakeBitmask(const typename std::underlying_type<T>::type &t)
	{
		return Bitmask<T>{t};
	}

	/**
	 * operator&(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const T &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		assert((static_cast<underlying_type>(lhs) & (static_cast<underlying_type>(lhs) - 1)) == 0);
		return Enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator&(Enumerator<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) & static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator&(Bitmask<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator&(const Bitmask<T> &lhs, const Bitmask<T> &rhs)
	{
		return Bitmask<T>{static_cast<T>(lhs.value & rhs.value)};
	}

	/**
	 * operator&(Bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const Bitmask<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Enumerator<T>{static_cast<T>(lhs.m_value& static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator&(T, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const T &lhs, const Bitmask<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs) & rhs.m_value)};
	}

	/**
	 * operator&(Bitmask<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const Bitmask<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Enumerator<T>{static_cast<T>(lhs.m_value& static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator&(Enumerator<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::Ealue, Enumerator<T>>::type operator&(const Enumerator<T> &lhs, const Bitmask<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) & rhs.m_value)};
	}

	/**
	 * operator&(T, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const T &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator&(Enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Enumerator<T>>::type operator&(const Enumerator<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) & static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator|(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const T &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator|(Enumerator<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) | static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator|(Bitmask<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const Bitmask<T> &lhs, const Bitmask<T> &rhs)
	{
		return Bitmask<T>{static_cast<T>(lhs.m_value | rhs.m_value)};
	}

	/**
	 * operator|(Bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const Bitmask<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(lhs.m_value | static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator|(T, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const T &lhs, const Bitmask<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) | rhs.m_value)};
	}

	/**
	 * operator|(Bitmask<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const Bitmask<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(lhs.m_value | static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator|(Enumerator<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const Enumerator<T> &lhs, const Bitmask<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) | rhs.m_value)};
	}

	/**
	 * operator|(Enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const Enumerator<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) | static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator|(T, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator|(const T &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator^(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const T &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator^(Enumerator<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) ^ static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator^(Bitmask<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const Bitmask<T> &lhs, const Bitmask<T> &rhs)
	{
		return Bitmask<T>{static_cast<T>(lhs.m_value^ rhs.m_value)};
	}

	/**
	 * operator^(Bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const Bitmask<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(lhs.m_value^ static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator^(T, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const T &lhs, const Bitmask<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) ^ rhs.m_value)};
	}

	/**
	 * operator^(Bitmask<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const Bitmask<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(lhs.m_value^ static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator^(Enumerator<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const Enumerator<T> &lhs, const Bitmask<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) ^ rhs.m_value)};
	}

	/**
	 * operator^(Enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const Enumerator<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.m_value) ^ static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator^(T, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator^(const T &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs.m_value))};
	}

	/**
	 * operator~(T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator~(const T &value)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(~ static_cast<underlying_type>(value))};
	}

	/**
	 * operator~(Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator~(const Enumerator<T> &lhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return Bitmask<T>{static_cast<T>(~ static_cast<underlying_type>(lhs.m_value))};
	}

	/**
	 * operator~(Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>>::type operator~(const Bitmask<T> &lhs)
	{
		return Bitmask<T>{static_cast<T>(~ lhs.m_value)};
	}

	/**
	 * operator&=(Bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator&=(Bitmask<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.m_value &= static_cast<underlying_type>(rhs);
		return lhs;
	}

	/**
	 * operator&=(Bitmask<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator&=(Bitmask<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.m_value &= static_cast<underlying_type>(rhs.m_value);
		return lhs;
	}

	/**
	 * operator&=(Bitmask<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator&=(Bitmask<T> &lhs, const Bitmask<T> &rhs)
	{
		lhs.m_value &= rhs.m_value;
		return lhs;
	}

	/**
	 * operator|=(Bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator|=(Bitmask<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.m_value |= static_cast<underlying_type>(rhs);
		return lhs;
	}

	/**
	 * operator|=(Bitmask<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator|=(Bitmask<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.m_value |= static_cast<underlying_type>(rhs.m_value);
		return lhs;
	}

	/**
	 * operator|=(Bitmask<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator|=(Bitmask<T> &lhs, const Bitmask<T> &rhs)
	{
		lhs.m_value |= rhs.m_value;
		return lhs;
	}

	/**
	 * operator^=(Bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator^=(Bitmask<T> &lhs, const T &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.m_value ^= static_cast<underlying_type>(rhs);
		return lhs;
	}

	/**
	 * operator^=(Bitmask<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator^=(Bitmask<T> &lhs, const Enumerator<T> &rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.m_value ^= static_cast<underlying_type>(rhs.m_value);
		return lhs;
	}

	/**
	 * operator^=(Bitmask<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, Bitmask<T>&>::type operator^=(Bitmask<T> &lhs, const Bitmask<T> &rhs)
	{
		lhs.m_value ^= rhs.m_value;
		return lhs;
	}

	/**
	 * operator==(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const T &lhs, const T &rhs)
	{
		return lhs == rhs;
	}

	/**
	 * operator==(Enumerator<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
	{
		return lhs.m_value == rhs.m_value;
	}

	/**
	 * operator==(Bitmask<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Bitmask<T> &lhs, const Bitmask<T> &rhs)
	{
		return lhs.m_value == rhs.m_value;
	}

	/**
	 * operator==(Enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Enumerator<T> &lhs, const T &rhs)
	{
		return lhs.m_value == rhs;
	}

	/**
	 * operator==(T, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const T &lhs, const Enumerator<T> &rhs)
	{
		return lhs == rhs.m_value;
	}

	/**
	 * operator==(Bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Bitmask<T> &lhs, const T &rhs)
	{
		static_assert(!std::is_same<typename Bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator==(T, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const T &lhs, const Bitmask<T> &rhs)
	{
		static_assert(!std::is_same<typename Bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator==(Bitmask<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Bitmask<T> &lhs, const Enumerator<T> &rhs)
	{
		static_assert(!std::is_same<typename Bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator==(Enumerator<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator==(const Enumerator<T> &lhs, const Bitmask<T> &rhs)
	{
		static_assert(!std::is_same<typename Bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator!=(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const T &lhs, const T &rhs)
	{
		return lhs != rhs;
	}

	/**
	 * operator!=(Enumerator<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Enumerator<T> &lhs, const Enumerator<T> &rhs)
	{
		return lhs.m_value != rhs.m_value;
	}

	/**
	 * operator!=(Bitmask<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Bitmask<T> &lhs, const Bitmask<T> &rhs)
	{
		return lhs.m_value != rhs.m_value;
	}

	/**
	 * operator!=(Enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Enumerator<T> &lhs, const T &rhs)
	{
		return lhs.m_value != rhs;
	}

	/**
	 * operator!=(T, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const T &lhs, const Enumerator<T> &rhs)
	{
		return lhs != rhs.value;
	}

	/**
	 * operator!=(Bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Bitmask<T> &lhs, const T &rhs)
	{
		static_assert(!std::is_same<typename Bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator!=(T, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const T &lhs, const Bitmask<T> &rhs)
	{
		static_assert(!std::is_same<typename Bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator!=(Bitmask<T>, Enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Bitmask<T> &lhs, const Enumerator<T> &rhs)
	{
		static_assert(!std::is_same<typename Bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator!=(Enumerator<T>, Bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type operator!=(const Enumerator<T> &lhs, const Bitmask<T> &rhs)
	{
		static_assert(!std::is_same<typename Bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
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
