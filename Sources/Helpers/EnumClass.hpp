// BSD 2-Clause License
//
// Copyright (c) 2017, Gabriel Aubut-Lussier
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
//     modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//     list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//     IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//     FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//     DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// https://dalzhim.github.io/2017/08/11/Improving-the-enum-class-bitmask

#pragma once

#include <cassert>
#include <type_traits>

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
	struct enumerator
	{
		constexpr enumerator(const T& value) : 
			value(value)
		{
		}
		
		constexpr explicit operator bool() const
		{
			using underlying_type = typename std::underlying_type<T>::type;
			return static_cast<underlying_type>(value) != 0;
		}
		
		constexpr operator T() const
		{
			return value;
		}

		T value;
	};

	/**
	 * Wrapper that differentiates combined enumerators from a single enumerator
	 * to provent accidental comparisons between a bitmask and a single enumerator
	 * using operator== or operator!=
	 */
	template<typename T>
	struct bitmask
	{
		using underlying_type = typename std::underlying_type<T>::type;

		constexpr bitmask(const T& value) : 
			value(static_cast<underlying_type>(value)) 
		{
		}
		
		constexpr bitmask(const enumerator<T>& enumerator) : 
			value(static_cast<underlying_type>(enumerator.value))
		{
		}
		
		constexpr explicit operator bool() const
		{
			return value != 0;
		}

		underlying_type value;
	};

	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	make_bitmask(const T& t)
	{
		return bitmask<T>{t};
	}

	/**
	 * operator&(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, enumerator<T>>::type
	operator&(const T& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		assert((static_cast<underlying_type>(lhs) & (static_cast<underlying_type>(lhs) - 1)) == 0);
		return enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator&(enumerator<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, enumerator<T>>::type
	operator&(const enumerator<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) & static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator&(bitmask<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator&(const bitmask<T>& lhs, const bitmask<T>& rhs)
	{
		return bitmask<T>{static_cast<T>(lhs.value & rhs.value)};
	}

	/**
	 * operator&(bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, enumerator<T>>::type
	operator&(const bitmask<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return enumerator<T>{static_cast<T>(lhs.value & static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator&(T, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, enumerator<T>>::type
	operator&(const T& lhs, const bitmask<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs) & rhs.value)};
	}

	/**
	 * operator&(bitmask<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, enumerator<T>>::type
	operator&(const bitmask<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return enumerator<T>{static_cast<T>(lhs.value & static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator&(enumerator<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, enumerator<T>>::type
	operator&(const enumerator<T>& lhs, const bitmask<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) & rhs.value)};
	}

	/**
	 * operator&(T, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, enumerator<T>>::type
	operator&(const T& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator&(enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, enumerator<T>>::type
	operator&(const enumerator<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return enumerator<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) & static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator|(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const T& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator|(enumerator<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const enumerator<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) | static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator|(bitmask<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const bitmask<T>& lhs, const bitmask<T>& rhs)
	{
		return bitmask<T>{static_cast<T>(lhs.value | rhs.value)};
	}

	/**
	 * operator|(bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const bitmask<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(lhs.value | static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator|(T, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const T& lhs, const bitmask<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) | rhs.value)};
	}

	/**
	 * operator|(bitmask<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const bitmask<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(lhs.value | static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator|(enumerator<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const enumerator<T>& lhs, const bitmask<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) | rhs.value)};
	}

	/**
	 * operator|(enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const enumerator<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) | static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator|(T, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator|(const T& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator^(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const T& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator^(enumerator<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const enumerator<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) ^ static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator^(bitmask<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const bitmask<T>& lhs, const bitmask<T>& rhs)
	{
		return bitmask<T>{static_cast<T>(lhs.value ^ rhs.value)};
	}

	/**
	 * operator^(bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const bitmask<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(lhs.value ^ static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator^(T, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const T& lhs, const bitmask<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) ^ rhs.value)};
	}

	/**
	 * operator^(bitmask<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const bitmask<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(lhs.value ^ static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator^(enumerator<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const enumerator<T>& lhs, const bitmask<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) ^ rhs.value)};
	}

	/**
	 * operator^(enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const enumerator<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs.value) ^ static_cast<underlying_type>(rhs))};
	}

	/**
	 * operator^(T, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator^(const T& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs.value))};
	}

	/**
	 * operator~(T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator~(const T& value)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(~ static_cast<underlying_type>(value))};
	}

	/**
	 * operator~(enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator~(const enumerator<T>& lhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		return bitmask<T>{static_cast<T>(~ static_cast<underlying_type>(lhs.value))};
	}

	/**
	 * operator~(bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>>::type
	operator~(const bitmask<T>& lhs)
	{
		return bitmask<T>{static_cast<T>(~ lhs.value)};
	}

	/**
	 * operator&=(bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator&=(bitmask<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.value &= static_cast<underlying_type>(rhs);
		return lhs;
	}

	/**
	 * operator&=(bitmask<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator&=(bitmask<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.value &= static_cast<underlying_type>(rhs.value);
		return lhs;
	}

	/**
	 * operator&=(bitmask<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator&=(bitmask<T>& lhs, const bitmask<T>& rhs)
	{
		lhs.value &= rhs.value;
		return lhs;
	}

	/**
	 * operator|=(bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator|=(bitmask<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.value |= static_cast<underlying_type>(rhs);
		return lhs;
	}

	/**
	 * operator|=(bitmask<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator|=(bitmask<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.value |= static_cast<underlying_type>(rhs.value);
		return lhs;
	}

	/**
	 * operator|=(bitmask<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator|=(bitmask<T>& lhs, const bitmask<T>& rhs)
	{
		lhs.value |= rhs.value;
		return lhs;
	}

	/**
	 * operator^=(bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator^=(bitmask<T>& lhs, const T& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.value ^= static_cast<underlying_type>(rhs);
		return lhs;
	}

	/**
	 * operator^=(bitmask<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator^=(bitmask<T>& lhs, const enumerator<T>& rhs)
	{
		using underlying_type = typename std::underlying_type<T>::type;
		lhs.value ^= static_cast<underlying_type>(rhs.value);
		return lhs;
	}

	/**
	 * operator^=(bitmask<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bitmask<T>&>::type
	operator^=(bitmask<T>& lhs, const bitmask<T>& rhs)
	{
		lhs.value ^= rhs.value;
		return lhs;
	}

	/**
	 * operator==(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const T& lhs, const T& rhs)
	{
		return lhs == rhs;
	}

	/**
	 * operator==(enumerator<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const enumerator<T>& lhs, const enumerator<T>& rhs)
	{
		return lhs.value == rhs.value;
	}

	/**
	 * operator==(bitmask<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const bitmask<T>& lhs, const bitmask<T>& rhs)
	{
		return lhs.value == rhs.value;
	}

	/**
	 * operator==(enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const enumerator<T>& lhs, const T& rhs)
	{
		return lhs.value == rhs;
	}

	/**
	 * operator==(T, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const T& lhs, const enumerator<T>& rhs)
	{
		return lhs == rhs.value;
	}

	/**
	 * operator==(bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const bitmask<T>& lhs, const T& rhs)
	{
		static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator==(T, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const T& lhs, const bitmask<T>& rhs)
	{
		static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator==(bitmask<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const bitmask<T>& lhs, const enumerator<T>& rhs)
	{
		static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator==(enumerator<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator==(const enumerator<T>& lhs, const bitmask<T>& rhs)
	{
		static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator!=(T, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const T& lhs, const T& rhs)
	{
		return lhs != rhs;
	}

	/**
	 * operator!=(enumerator<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const enumerator<T>& lhs, const enumerator<T>& rhs)
	{
		return lhs.value != rhs.value;
	}

	/**
	 * operator!=(bitmask<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const bitmask<T>& lhs, const bitmask<T>& rhs)
	{
		return lhs.value != rhs.value;
	}

	/**
	 * operator!=(enumerator<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const enumerator<T>& lhs, const T& rhs)
	{
		return lhs.value != rhs;
	}

	/**
	 * operator!=(T, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const T& lhs, const enumerator<T>& rhs)
	{
		return lhs != rhs.value;
	}

	/**
	 * operator!=(bitmask<T>, T)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const bitmask<T>& lhs, const T& rhs)
	{
		static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator!=(T, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const T& lhs, const bitmask<T>& rhs)
	{
		static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator!=(bitmask<T>, enumerator<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const bitmask<T>& lhs, const enumerator<T>& rhs)
	{
		static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}

	/**
	 * operator!=(enumerator<T>, bitmask<T>)
	 */
	template<typename T>
	constexpr
	typename std::enable_if<std::is_enum<T>::value && EnableBitMaskOperators<T>::value, bool>::type
	operator!=(const enumerator<T>& lhs, const bitmask<T>& rhs)
	{
		static_assert(!std::is_same<typename bitmask<T>::underlying_type, typename std::underlying_type<T>::type>::value, "A bitmask can't be compared to an enumerator. Use & first.");
		return false;
	}
}