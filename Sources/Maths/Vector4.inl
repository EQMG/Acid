#pragma once

#include "Files/Node.hpp"
#include "Maths.hpp"
#include "Vector4.hpp"

namespace acid {
template<typename T>
constexpr Vector4<T>::Vector4(const T &a):
	x(a),
	y(a),
	z(a),
	w(a) {
}

template<typename T>
constexpr Vector4<T>::Vector4(const T &x, const T &y, const T &z, const T &w):
	x(x),
	y(y),
	z(z),
	w(w) {
}

template<typename T>
template<typename K, typename J, typename H, typename F>
constexpr Vector4<T>::Vector4(const K &x, const J &y, const H &z, const F &w) :
	x(static_cast<T>(x)),
	y(static_cast<T>(y)),
	z(static_cast<T>(z)),
	w(static_cast<T>(w)) {
}

template<typename T>
template<typename K, typename J>
constexpr Vector4<T>::Vector4(const Vector2<K> &left, const Vector2<J> &right):
	x(static_cast<T>(left.x)),
	y(static_cast<T>(left.y)),
	z(static_cast<T>(right.x)),
	w(static_cast<T>(right.y)) {
}

template<typename T>
template<typename K, typename J>
constexpr Vector4<T>::Vector4(const Vector3<K> &source, const J &w):
	x(static_cast<T>(source.x)),
	y(static_cast<T>(source.y)),
	z(static_cast<T>(source.z)),
	w(static_cast<T>(w)) {
}

template<typename T>
template<typename K>
constexpr Vector4<T>::Vector4(const Vector4<K> &source):
	x(static_cast<T>(source.x)),
	y(static_cast<T>(source.y)),
	z(static_cast<T>(source.z)),
	w(static_cast<T>(source.w)) {
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::Add(const Vector4<K> &other) const {
	return Vector4<decltype(x + other.x)>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::Subtract(const Vector4<K> &other) const {
	return Vector4<decltype(x - other.x)>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::Multiply(const Vector4<K> &other) const {
	return Vector4<decltype(x * other.x)>(x * other.x, y * other.y, z * other.z, w * other.w);
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::Divide(const Vector4<K> &other) const {
	return Vector4<decltype(x / other.x)>(x / other.x, y / other.y, z / other.z, w / other.w);
}

template<typename T>
template<typename K>
auto Vector4<T>::Angle(const Vector4<K> &other) const {
	auto dls = Dot(other) / (Length() * other.Length());

	if (dls < -1) {
		dls = -1;
	} else if (dls > 1) {
		dls = 1;
	}

	return std::acos(dls);
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::Dot(const Vector4<K> &other) const {
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector4<T>::Lerp(const Vector4<K> &other, const J &progression) const {
	auto ta = *this * (1 - progression);
	auto tb = other * progression;
	return ta + tb;
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::Scale(const K &scalar) const {
	return Vector4<decltype(x * scalar)>(x * scalar, y * scalar, z * scalar, w * scalar);
}

template<typename T>
auto Vector4<T>::Normalize() const {
	auto l = Length();

	if (l == 0) {
		throw std::runtime_error("Can't normalize a zero length vector");
	}

	return *this / l;
}

template<typename T>
constexpr auto Vector4<T>::LengthSquared() const {
	return x * x + y * y + z * z + w * w;
}

template<typename T>
auto Vector4<T>::Length() const {
	return std::sqrt(LengthSquared());
}

template<typename T>
auto Vector4<T>::Abs() const {
	return Vector2<T>(std::abs(x), std::abs(y), std::abs(z), std::abs(w));
}

template<typename T>
constexpr auto Vector4<T>::Min() const {
	return std::min({x, y, z, w});
}

template<typename T>
constexpr auto Vector4<T>::Max() const {
	return std::max({x, y, z, w});
}

template<typename T>
constexpr auto Vector4<T>::MinMax() const {
	return std::minmax({x, y, z, w});
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::Min(const Vector4<K> &other) {
	using THighestP = decltype(x + other.x);
	return Vector4<THighestP>(std::min<THighestP>(x, other.x), std::min<THighestP>(y, other.y), std::min<THighestP>(z, other.z),
		std::min<THighestP>(w, other.w));
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::Max(const Vector4<K> &other) {
	using THighestP = decltype(x + other.x);
	return Vector4<THighestP>(std::max<THighestP>(x, other.x), std::max<THighestP>(y, other.y), std::max<THighestP>(z, other.z),
		std::max<THighestP>(w, other.w));
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::DistanceSquared(const Vector4<K> &other) const {
	auto dx = x - other.x;
	auto dy = y - other.y;
	auto dz = z - other.z;
	auto dw = w - other.w;
	return dx * dx + dy * dy + dz * dz + dw * dw;
}

template<typename T>
template<typename K>
auto Vector4<T>::Distance(const Vector4<K> &other) const {
	return std::sqrt(DistanceSquared(other));
}

template<typename T>
template<typename K>
constexpr auto Vector4<T>::DistanceVector(const Vector4<K> &other) const {
	return (*this - other) * (*this - other);
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector4<T>::SmoothDamp(const Vector4<K> &target, const Vector4<J> &rate) const {
	return Maths::SmoothDamp(*this, target, rate);
}

template<typename T>
constexpr const T &Vector4<T>::operator[](uint32_t index) const {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::runtime_error("Vector4 index out of bounds!");
	}
}

template<typename T>
constexpr T &Vector4<T>::operator[](uint32_t index) {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::runtime_error("Vector4 index out of bounds!");
	}
}

template<typename T>
template<typename K>
constexpr bool Vector4<T>::operator==(const Vector4<K> &other) const {
	return x == other.x && y == other.y && z == other.z && w == other.w;
}

template<typename T>
template<typename K>
constexpr bool Vector4<T>::operator!=(const Vector4<K> &other) const {
	return !operator==(other);
}

template<typename T>
template<typename U>
constexpr std::enable_if_t<std::is_signed_v<U>, Vector4<T>> Vector4<T>::operator-() const {
	return {-x, -y, -z, -w};
}

template<typename T>
template<typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Vector4<T>> Vector4<T>::operator~() const {
	return {~x, ~y, ~z, ~w};
}

template<typename T>
template<typename K>
constexpr Vector4<T> &Vector4<T>::operator+=(const Vector4<K> &other) {
	return *this = Add(other);
}

template<typename T>
template<typename K>
constexpr Vector4<T> &Vector4<T>::operator-=(const Vector4<K> &other) {
	return *this = Subtract(other);
}

template<typename T>
template<typename K>
constexpr Vector4<T> &Vector4<T>::operator*=(const Vector4<K> &other) {
	return *this = Multiply(other);
}

template<typename T>
template<typename K>
constexpr Vector4<T> &Vector4<T>::operator/=(const Vector4<K> &other) {
	return *this = Divide(other);
}

template<typename T>
constexpr Vector4<T> &Vector4<T>::operator+=(const T &other) {
	return *this = Add(Vector4<T>(other));
}

template<typename T>
constexpr Vector4<T> &Vector4<T>::operator-=(const T &other) {
	return *this = Subtract(Vector4<T>(other));
}

template<typename T>
constexpr Vector4<T> &Vector4<T>::operator*=(const T &other) {
	return *this = Multiply(Vector4<T>(other));
}

template<typename T>
constexpr Vector4<T> &Vector4<T>::operator/=(const T &other) {
	return *this = Divide(Vector4<T>(other));
}

template<typename K>
const Node &operator>>(const Node &node, Vector4<K> &vector) {
	node["x"].Get(vector.x);
	node["y"].Get(vector.y);
	node["z"].Get(vector.z);
	node["w"].Get(vector.w);
	return node;
}

template<typename K>
Node &operator<<(Node &node, const Vector4<K> &vector) {
	node["x"].Set(vector.x);
	node["y"].Set(vector.y);
	node["z"].Set(vector.z);
	node["w"].Set(vector.w);
	return node;
}

template<typename K>
std::ostream &operator<<(std::ostream &stream, const Vector4<K> &vector) {
	return stream << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w;
}

template<typename K, typename J>
constexpr auto operator+(const Vector4<K> &left, const Vector4<J> &right) {
	return left.Add(right);
}

template<typename K, typename J>
constexpr auto operator-(const Vector4<K> &left, const Vector4<J> &right) {
	return left.Subtract(right);
}

template<typename K, typename J>
constexpr auto operator*(const Vector4<K> &left, const Vector4<J> &right) {
	return left.Multiply(right);
}

template<typename K, typename J>
constexpr auto operator/(const Vector4<K> &left, const Vector4<J> &right) {
	return left.Divide(right);
}

template<typename K, typename J>
constexpr auto operator+(const K &left, const Vector4<J> &right) {
	return Vector4<K>(left).Add(right);
}

template<typename K, typename J>
constexpr auto operator-(const K &left, const Vector4<J> &right) {
	return Vector4<K>(left).Subtract(right);
}

template<typename K, typename J>
constexpr auto operator*(const K &left, const Vector4<J> &right) {
	return Vector4<K>(left).Multiply(right);
}

template<typename K, typename J>
constexpr auto operator/(const K &left, const Vector4<J> &right) {
	return Vector4<K>(left).Divide(right);
}

template<typename K, typename J>
constexpr auto operator+(const Vector4<K> &left, const J &right) {
	return left.Add(Vector4<J>(right));
}

template<typename K, typename J>
constexpr auto operator-(const Vector4<K> &left, const J &right) {
	return left.Subtract(Vector4<J>(right));
}

template<typename K, typename J>
constexpr auto operator*(const Vector4<K> &left, const J &right) {
	return left.Multiply(Vector4<J>(right));
}

template<typename K, typename J>
constexpr auto operator/(const Vector4<K> &left, const J &right) {
	return left.Divide(Vector4<J>(right));
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator&(const Vector4<K> &left, const Vector4<J> &right) {
	return {left.x & right.x, left.y & right.y, left.z & right.z, left.w & right.w};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator|(const Vector4<K> &left, const Vector4<J> &right) {
	return {left.x | right.x, left.y | right.y, left.z | right.z, left.w | right.w};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator>>(const Vector4<K> &left, const Vector4<J> &right) {
	return {left.x >> right.x, left.y >> right.y, left.z >> right.z, left.w >> right.w};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator<<(const Vector4<K> &left, const Vector4<J> &right) {
	return {left.x << right.x, left.y << right.y, left.z << right.z, left.w << right.w};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator&(const Vector4<K> &left, const J &right) {
	return {left.x & right, left.y & right, left.z & right, left.w & right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator|(const Vector4<K> &left, const J &right) {
	return {left.x | right, left.y | right, left.z | right, left.w | right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator>>(const Vector4<K> &left, const J &right) {
	return {left.x >> right, left.y >> right, left.z >> right, left.w >> right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator<<(const Vector4<K> &left, const J &right) {
	return {left.x << right, left.y << right, left.z << right, left.w << right};
}

}

namespace std {
template<typename T>
struct hash<acid::Vector4<T>> {
	size_t operator()(const acid::Vector4<T> &vector) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.x);
		acid::Maths::HashCombine(seed, vector.y);
		acid::Maths::HashCombine(seed, vector.z);
		acid::Maths::HashCombine(seed, vector.w);
		return seed;
	}
};
}
