#pragma once

#include "Vector.hpp"

#include <type_traits>

#include "Files/Node.hpp"
#include "Maths.hpp"

namespace acid {
template<typename T, std::size_t N>
const Vector<T, N> Vector<T, N>::Zero = Vector<T, N>(0);
template<typename T, std::size_t N>
const Vector<T, N> Vector<T, N>::One = Vector<T, N>(1);
template<typename T, std::size_t N>
const Vector<T, N> Vector<T, N>::Infinity = Vector<T, N>(std::numeric_limits<T>::infinity());
template<typename T, std::size_t N> // typename = std::enable_if_t<N <= 2> 
const Vector<T, N> Vector<T, N>::Right = Vector<T, 2>(1, 0);
template<typename T, std::size_t N> // typename = std::enable_if_t<N <= 2> 
const Vector<T, N> Vector<T, N>::Left = Vector<T, 2>(-1, 0);
template<typename T, std::size_t N> // typename = std::enable_if_t<N <= 2> 
const Vector<T, N> Vector<T, N>::Up = Vector<T, 2>(0, 1);
template<typename T, std::size_t N> // typename = std::enable_if_t<N <= 2> 
const Vector<T, N> Vector<T, N>::Down = Vector<T, 2>(0, -1);
template<typename T, std::size_t N> // typename = std::enable_if_t<N <= 3> 
const Vector<T, N> Vector<T, N>::Front = Vector<T, 3>(0, 0, 1);
template<typename T, std::size_t N> // typename = std::enable_if_t<N <= 3> 
const Vector<T, N> Vector<T, N>::Back = Vector<T, 3>(0, 0, -1);

template<typename T1, typename T2, std::size_t N>
constexpr auto operator==(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	for (std::size_t i = 0; i < N; i++) {
		if (lhs[i] != rhs[i])
			return false;
	}
	return true;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator!=(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	for (std::size_t i = 0; i < N; i++) {
		if (lhs[i] != rhs[i])
			return true;
	}
	return false;
}

template<typename T, std::size_t N>
constexpr auto operator+(const Vector<T, N> &lhs) {
	Vector<T, N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = +lhs[i];
	return result;
}

template<typename T, std::size_t N>
constexpr auto operator-(const Vector<T, N> &lhs) {
	Vector<T, N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = -lhs[i];
	return result;
}

template<typename T, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T>>>
constexpr auto operator~(const Vector<T, N> &lhs) {
	Vector<T, N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = ~lhs[i];
	return result;
}

template<typename T, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T>>>
constexpr auto operator!(const Vector<T, N> &lhs) {
	Vector<T, N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = !lhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator+(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] + rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] + rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator-(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] - rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] - rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator*(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] * rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] * rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator/(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] / rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] / rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator%(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] % rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] & rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator|(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] | rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] | rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator^(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] ^ rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] ^ rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator&(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] & rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] & rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator<<(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] << rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] << rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator>>(const Vector<T1, N> &lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs[0] >> rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] >> rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator*(const Vector<T1, N> &lhs, T2 rhs) {
	Vector<decltype(lhs[0] * rhs), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] * rhs;
	return result;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator/(const Vector<T1, N> &lhs, T2 rhs) {
	Vector<decltype(lhs[0] / rhs), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] / rhs;
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator%(const Vector<T1, N> &lhs, T2 rhs) {
	Vector<decltype(lhs[0] % rhs), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] & rhs;
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator|(const Vector<T1, N> &lhs, T2 rhs) {
	Vector<decltype(lhs[0] | rhs), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] | rhs;
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator^(const Vector<T1, N> &lhs, T2 rhs) {
	Vector<decltype(lhs[0] ^ rhs), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] ^ rhs;
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator&(const Vector<T1, N> &lhs, T2 rhs) {
	Vector<decltype(lhs[0] & rhs), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] & rhs;
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator<<(const Vector<T1, N> &lhs, T2 rhs) {
	Vector<decltype(lhs[0] << rhs), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] << rhs;
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator>>(const Vector<T1, N> &lhs, T2 rhs) {
	Vector<decltype(lhs[0] >> rhs), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs[i] >> rhs;
	return result;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator*(T1 lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs * rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs * rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator/(T1 lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs / rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs / rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator%(T1 lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs % rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs & rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator|(T1 lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs | rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs | rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator^(T1 lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs ^ rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs ^ rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator&(T1 lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs & rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs & rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator<<(T1 lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs << rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs << rhs[i];
	return result;
}

template<typename T1, typename T2, std::size_t N, typename = std::enable_if_t<std::is_integral_v<T1> && std::is_integral_v<T2>>>
constexpr auto operator>>(T1 lhs, const Vector<T2, N> &rhs) {
	Vector<decltype(lhs >> rhs[0]), N> result;
	for (std::size_t i = 0; i < N; i++)
		result[i] = lhs >> rhs[i];
	return result;
}


template<typename T1, typename T2, std::size_t N>
constexpr auto operator+=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs + rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator-=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs - rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator*=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs * rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator/=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs / rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator%=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs % rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator|=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs | rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator^=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs ^ rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator&=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs & rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator<<=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs << rhs;
}

template<typename T1, typename T2, std::size_t N>
constexpr auto operator>>=(Vector<T1, N> &lhs, const T2 &rhs) {
	return lhs = lhs >> rhs;
}

template<typename T, std::size_t N>
std::ostream &operator<<(std::ostream &stream, const Vector<T, N> &vector) {
	for (std::size_t i = 0; i < N; i++)
		stream << vector[i] << (i != N - 1 ? ", " : "");
	return stream;
}

template<typename T, std::size_t N>
const Node &operator>>(const Node &node, Vector<T, N> &vector) {
	if constexpr (N >= 1) 
		node["x"].Get(vector.x);
	if constexpr (N >= 2)
		node["y"].Get(vector.y);
	if constexpr (N >= 3)
		node["z"].Get(vector.z);
	if constexpr (N >= 4)
		node["w"].Get(vector.w);
	for (std::size_t i = 4; i < N; i++)
		node["n" + std::to_string(i)].Get(vector[i]);
	return node;
}

template<typename T, std::size_t N>
Node &operator<<(Node &node, const Vector<T, N> &vector) {
	if constexpr (N >= 1)
		node["x"].Set(vector.x);
	if constexpr (N >= 2)
		node["y"].Set(vector.y);
	if constexpr (N >= 3)
		node["z"].Set(vector.z);
	if constexpr (N >= 4)
		node["w"].Set(vector.w);
	for (std::size_t i = 4; i < N; i++)
		node["n" + std::to_string(i)].Set(vector[i]);
	return node;
}
}

namespace std {
template<typename T, size_t N>
struct hash<acid::Vector<T, N>> {
	size_t operator()(const acid::Vector<T, N> &vector) const noexcept {
		size_t seed = 0;
		for (size_t i = 0; i < N; i++)
			acid::Maths::HashCombine(seed, vector[i]);
		return seed;
	}
};
}
