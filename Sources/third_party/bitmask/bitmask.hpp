#pragma once

#include <type_traits>

namespace bitmask {
// https://dalzhim.github.io/2017/08/11/Improving-the-enum-class-bitmask
template<typename Enum>
struct enable_bitmask_operators : std::false_type {
};

template<typename Enum>
inline constexpr bool enable_bitmask_operators_v = enable_bitmask_operators<Enum>::value;

template<typename Enum>
using enable_if_t = std::enable_if_t<std::is_enum_v<Enum> && enable_bitmask_operators_v<Enum>>;

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

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const T &lhs, const T &rhs) {
	return lhs == rhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const enumerator<T> &lhs, const enumerator<T> &rhs) {
	return lhs.value == rhs.value;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const bitmask<T> &lhs, const bitmask<T> &rhs) {
	return lhs.value == rhs.value;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const enumerator<T> &lhs, const T &rhs) {
	return lhs.value == rhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const T &lhs, const enumerator<T> &rhs) {
	return lhs == rhs.value;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const bitmask<T> &lhs, const T &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const T &lhs, const bitmask<T> &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const enumerator<T> &lhs, const bitmask<T> &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator==(const bitmask<T> &lhs, const enumerator<T> &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const T &lhs, const T &rhs) {
	return lhs != rhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const enumerator<T> &lhs, const enumerator<T> &rhs) {
	return lhs.value != rhs.value;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const bitmask<T> &lhs, const bitmask<T> &rhs) {
	return lhs.value != rhs.value;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const enumerator<T> &lhs, const T &rhs) {
	return lhs.value != rhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const T &lhs, const enumerator<T> &rhs) {
	return lhs != rhs.value;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const bitmask<T> &lhs, const T &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const T &lhs, const bitmask<T> &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const enumerator<T> &lhs, const bitmask<T> &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T, typename = enable_if_t<T>>
constexpr bool operator!=(const bitmask<T> &lhs, const enumerator<T> &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return false;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator~(const T &value) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(~static_cast<underlying_type>(value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator~(const enumerator<T> &lhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(~static_cast<underlying_type>(lhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator~(const bitmask<T> &lhs) {
	return {static_cast<T>(~lhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr enumerator<T> operator&(const T &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	assert((static_cast<underlying_type>(lhs) & (static_cast<underlying_type>(lhs) - 1)) == 0);
	return {static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr enumerator<T> operator&(const enumerator<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator&(const bitmask<T> &lhs, const bitmask<T> &rhs) {
	return {static_cast<T>(lhs.value & rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr enumerator<T> operator&(const enumerator<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr enumerator<T> operator&(const T &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr enumerator<T> operator&(const bitmask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value & static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr enumerator<T> operator&(const T &lhs, const bitmask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) & rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr enumerator<T> operator&(const enumerator<T> &lhs, const bitmask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) & rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr enumerator<T> operator&(const bitmask<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value & static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const T &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const enumerator<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const bitmask<T> &lhs, const bitmask<T> &rhs) {
	return {static_cast<T>(lhs.value | rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const enumerator<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const T &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const bitmask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value | static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const T &lhs, const bitmask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) | rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const enumerator<T> &lhs, const bitmask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) | rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator|(const bitmask<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value | static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const T &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const enumerator<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const bitmask<T> &lhs, const bitmask<T> &rhs) {
	return {static_cast<T>(lhs.value ^ rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const enumerator<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const T &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const bitmask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value ^ static_cast<underlying_type>(rhs))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const T &lhs, const bitmask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs) ^ rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const enumerator<T> &lhs, const bitmask<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(static_cast<underlying_type>(lhs.value) ^ rhs.value)};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> operator^(const bitmask<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	return {static_cast<T>(lhs.value ^ static_cast<underlying_type>(rhs.value))};
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator&=(bitmask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value &= static_cast<underlying_type>(rhs);
	return lhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator&=(bitmask<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value &= static_cast<underlying_type>(rhs.value);
	return lhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator&=(bitmask<T> &lhs, const bitmask<T> &rhs) {
	lhs.value &= rhs.value;
	return lhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator|=(bitmask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value |= static_cast<underlying_type>(rhs);
	return lhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator|=(bitmask<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value |= static_cast<underlying_type>(rhs.value);
	return lhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator|=(bitmask<T> &lhs, const bitmask<T> &rhs) {
	lhs.value |= rhs.value;
	return lhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator^=(bitmask<T> &lhs, const T &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value ^= static_cast<underlying_type>(rhs);
	return lhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator^=(bitmask<T> &lhs, const enumerator<T> &rhs) {
	using underlying_type = typename std::underlying_type_t<T>;
	lhs.value ^= static_cast<underlying_type>(rhs.value);
	return lhs;
}

template<typename T, typename = enable_if_t<T>>
constexpr bitmask<T> &operator^=(bitmask<T> &lhs, const bitmask<T> &rhs) {
	lhs.value ^= rhs.value;
	return lhs;
}

/*template<typename T, typename = enable_if_t<T>>
constexpr std::ostream &operator<<(std::ostream &lhs, const T &rhs) {
   return lhs << static_cast<std::underlying_type_t<T>>(rhs);
}

template<typename T, typename = enable_if_t<T>>
constexpr std::istream &operator>>(std::istream &lhs, const T &rhs) {
	return lhs >> static_cast<std::underlying_type_t<T>>(rhs);
}

template<typename T, typename = enable_if_t<T>>
constexpr std::ostream &operator<<(std::ostream &lhs, const bitmask<T> &rhs) {
   static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
   return lhs << rhs.value;
}

template<typename T, typename = enable_if_t<T>>
constexpr std::istream &operator>>(std::istream &lhs, const bitmask<T> &rhs) {
	static_assert(!std::is_same_v<typename bitmask<T>::underlying_type, std::underlying_type_t<T>>, "A bitmask can't be compared to an enumerator. Use & first.");
	return lhs >> rhs.value;
}*/

// https://stackoverflow.com/a/8498694/6251899
template<typename T>
struct enum_iterator {
	struct iterator {
		using underlying_type = typename std::underlying_type_t<T>;

		iterator(const underlying_type &value) :
			value(value) {
		}

		T operator*() const { return static_cast<T>(value); }
		void operator++() { ++value; }
		bool operator!=(iterator rhs) { return value != rhs.value; }

		underlying_type value;
	};
};

template<typename T>
typename enum_iterator<T>::iterator begin(enum_iterator<T>) {
	using underlying_type = typename std::underlying_type_t<T>;
	return typename enum_iterator<T>::iterator(static_cast<underlying_type>(T::First));
}

template<typename T>
typename enum_iterator<T>::iterator end(enum_iterator<T>) {
	using underlying_type = typename std::underlying_type_t<T>;
	return typename enum_iterator<T>::iterator(static_cast<underlying_type>(T::Last) + 1);
}
}
