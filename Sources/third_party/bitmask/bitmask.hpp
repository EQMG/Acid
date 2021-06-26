/**
 * BSD 2-Clause License
 * 
 * Copyright (c) 2017, Gabriel Aubut-Lussier
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * https://dalzhim.github.io/2017/08/11/Improving-the-enum-class-bitmask
 * https://stackoverflow.com/a/8498694/6251899
 **/

#pragma once

#include <type_traits>

#include <magic_enum/magic_enum.hpp>

namespace bitmask {
/**
 * @brief Wrapper for an enumerator that provides implicit bool conversion
 */
template<typename T>
struct enumerator {
	using underlying_type = typename std::underlying_type_t<T>;
	
	constexpr enumerator(const T &value) :
		value(value) {
	}

	constexpr operator bool() const {
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
struct bitmask {
	using underlying_type = typename std::underlying_type_t<T>;

	constexpr bitmask() :
		value(static_cast<underlying_type>(0)) {
	}

	constexpr bitmask(const T &value) :
		value(static_cast<underlying_type>(value)) {
	}

	constexpr bitmask(const underlying_type &value) :
		value(value) {
	}

	constexpr bitmask(const enumerator<T> &enumerator) :
		value(static_cast<underlying_type>(enumerator.value)) {
	}

	constexpr explicit operator bool() const { return value != 0; }

	underlying_type value;
};
}

#define ENABLE_BITMASK_OPERATORS(T)\
constexpr bool operator==(const bitmask::enumerator<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	return lhs.value == rhs.value;\
}\
constexpr bool operator==(const bitmask::bitmask<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	return lhs.value == rhs.value;\
}\
constexpr bool operator==(const bitmask::enumerator<T> &lhs, const T &rhs) {\
	return lhs.value == rhs;\
}\
constexpr bool operator==(const T &lhs, const bitmask::enumerator<T> &rhs) {\
	return lhs == rhs.value;\
}\
constexpr bool operator!=(const bitmask::enumerator<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	return lhs.value != rhs.value;\
}\
constexpr bool operator!=(const bitmask::bitmask<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	return lhs.value != rhs.value;\
}\
constexpr bool operator!=(const bitmask::enumerator<T> &lhs, const T &rhs) {\
	return lhs.value != rhs;\
}\
constexpr bool operator!=(const T &lhs, const bitmask::enumerator<T> &rhs) {\
	return lhs != rhs.value;\
}\
constexpr bitmask::bitmask<T> operator~(const T &value) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(~static_cast<underlying_type>(value))};\
}\
constexpr bitmask::bitmask<T> operator~(const bitmask::enumerator<T> &lhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(~static_cast<underlying_type>(lhs.value))};\
}\
constexpr bitmask::bitmask<T> operator~(const bitmask::bitmask<T> &lhs) {\
	return {static_cast<T>(~lhs.value)};\
}\
constexpr bitmask::enumerator<T> operator&(const T &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	assert((static_cast<underlying_type>(lhs) & (static_cast<underlying_type>(lhs) - 1)) == 0);\
	return {static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::enumerator<T> operator&(const bitmask::enumerator<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::bitmask<T> operator&(const bitmask::bitmask<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	return {static_cast<T>(lhs.value & rhs.value)};\
}\
constexpr bitmask::enumerator<T> operator&(const bitmask::enumerator<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::enumerator<T> operator&(const T &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::enumerator<T> operator&(const bitmask::bitmask<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(lhs.value & static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::enumerator<T> operator&(const T &lhs, const bitmask::bitmask<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs) & rhs.value)};\
}\
constexpr bitmask::enumerator<T> operator&(const bitmask::enumerator<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & rhs.value)};\
}\
constexpr bitmask::enumerator<T> operator&(const bitmask::bitmask<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(lhs.value & static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::bitmask<T> operator|(const T &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::bitmask<T> operator|(const bitmask::enumerator<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::bitmask<T> operator|(const bitmask::bitmask<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	return {static_cast<T>(lhs.value | rhs.value)};\
}\
constexpr bitmask::bitmask<T> operator|(const bitmask::enumerator<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::bitmask<T> operator|(const T &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::bitmask<T> operator|(const bitmask::bitmask<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(lhs.value | static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::bitmask<T> operator|(const T &lhs, const bitmask::bitmask<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs) | rhs.value)};\
}\
constexpr bitmask::bitmask<T> operator|(const bitmask::enumerator<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | rhs.value)};\
}\
constexpr bitmask::bitmask<T> operator|(const bitmask::bitmask<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(lhs.value | static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::bitmask<T> operator^(const T &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::bitmask<T> operator^(const bitmask::enumerator<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::bitmask<T> operator^(const bitmask::bitmask<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	return {static_cast<T>(lhs.value ^ rhs.value)};\
}\
constexpr bitmask::bitmask<T> operator^(const bitmask::enumerator<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::bitmask<T> operator^(const T &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::bitmask<T> operator^(const bitmask::bitmask<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(lhs.value ^ static_cast<underlying_type>(rhs))};\
}\
constexpr bitmask::bitmask<T> operator^(const T &lhs, const bitmask::bitmask<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ rhs.value)};\
}\
constexpr bitmask::bitmask<T> operator^(const bitmask::enumerator<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ rhs.value)};\
}\
constexpr bitmask::bitmask<T> operator^(const bitmask::bitmask<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	return {static_cast<T>(lhs.value ^ static_cast<underlying_type>(rhs.value))};\
}\
constexpr bitmask::bitmask<T> &operator&=(bitmask::bitmask<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	lhs.value &= static_cast<underlying_type>(rhs);\
	return lhs;\
}\
constexpr bitmask::bitmask<T> &operator&=(bitmask::bitmask<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	lhs.value &= static_cast<underlying_type>(rhs.value);\
	return lhs;\
}\
constexpr bitmask::bitmask<T> &operator&=(bitmask::bitmask<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	lhs.value &= rhs.value;\
	return lhs;\
}\
constexpr bitmask::bitmask<T> &operator|=(bitmask::bitmask<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	lhs.value |= static_cast<underlying_type>(rhs);\
	return lhs;\
}\
constexpr bitmask::bitmask<T> &operator|=(bitmask::bitmask<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	lhs.value |= static_cast<underlying_type>(rhs.value);\
	return lhs;\
}\
constexpr bitmask::bitmask<T> &operator|=(bitmask::bitmask<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	lhs.value |= rhs.value;\
	return lhs;\
}\
constexpr bitmask::bitmask<T> &operator^=(bitmask::bitmask<T> &lhs, const T &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	lhs.value ^= static_cast<underlying_type>(rhs);\
	return lhs;\
}\
constexpr bitmask::bitmask<T> &operator^=(bitmask::bitmask<T> &lhs, const bitmask::enumerator<T> &rhs) {\
	using underlying_type = typename std::underlying_type_t<T>;\
	lhs.value ^= static_cast<underlying_type>(rhs.value);\
	return lhs;\
}\
constexpr bitmask::bitmask<T> &operator^=(bitmask::bitmask<T> &lhs, const bitmask::bitmask<T> &rhs) {\
	lhs.value ^= rhs.value;\
	return lhs;\
}\
/*constexpr std::ostream &operator<<(std::ostream &lhs, const T &rhs) {\
   return lhs << static_cast<std::underlying_type_t<T>>(rhs);\
}\
constexpr std::istream &operator>>(std::istream &lhs, const T &rhs) {\
	return lhs >> static_cast<std::underlying_type_t<T>>(rhs);\
}\
constexpr std::ostream &operator<<(std::ostream &lhs, const bitmask::bitmask<T> &rhs) {\
   static_assert(!std::is_same_v<typename bitmask::bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");\
   return lhs << rhs.value;\
}\
constexpr std::istream &operator>>(std::istream &lhs, const bitmask::bitmask<T> &rhs) {\
	static_assert(!std::is_same_v<typename bitmask::bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");\
	return lhs >> rhs.value;\
}*/
