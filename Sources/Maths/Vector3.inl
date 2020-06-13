#pragma once

#include "Files/Node.hpp"
#include "Maths.hpp"
#include "Vector3.hpp"

namespace acid {
template<typename T>
constexpr Vector3<T>::Vector3(const T &a):
	x(a),
	y(a),
	z(a) {
}

template<typename T>
constexpr Vector3<T>::Vector3(const T &x, const T &y, const T &z):
	x(x),
	y(y),
	z(z) {
}

template<typename T>
template<typename K, typename J, typename H>
constexpr Vector3<T>::Vector3(const K &x, const J &y, const H &z) :
	x(static_cast<T>(x)),
	y(static_cast<T>(y)),
	z(static_cast<T>(z)) {
}

template<typename T>
template<typename K, typename J>
constexpr Vector3<T>::Vector3(const Vector2<K> &source, const J &z):
	x(static_cast<T>(source.x)),
	y(static_cast<T>(source.y)),
	z(static_cast<T>(z)) {
}

template<typename T>
template<typename K>
constexpr Vector3<T>::Vector3(const Vector3<K> &source):
	x(static_cast<T>(source.x)),
	y(static_cast<T>(source.y)),
	z(static_cast<T>(source.z)) {
}

template<typename T>
template<typename K>
constexpr Vector3<T>::Vector3(const Vector4<K> &source):
	x(static_cast<T>(source.x)),
	y(static_cast<T>(source.y)),
	z(static_cast<T>(source.z)) {
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Add(const Vector3<K> &other) const {
	return Vector3<decltype(x + other.x)>(x + other.x, y + other.y, z + other.z);
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Subtract(const Vector3<K> &other) const {
	return Vector3<decltype(x - other.x)>(x - other.x, y - other.y, z - other.z);
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Multiply(const Vector3<K> &other) const {
	return Vector3<decltype(x * other.x)>(x * other.x, y * other.y, z * other.z);
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Divide(const Vector3<K> &other) const {
	return Vector3<decltype(x / other.x)>(x / other.x, y / other.y, z / other.z);
}

template<typename T>
template<typename K>
auto Vector3<T>::Angle(const Vector3<K> &other) const {
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
constexpr auto Vector3<T>::Dot(const Vector3<K> &other) const {
	return x * other.x + y * other.y + z * other.z;
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Cross(const Vector3<K> &other) const {
	return Vector3<decltype(x * other.x)>(y * other.z - z * other.y, other.x * z - other.z * x, x * other.y - y * other.x);
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector3<T>::Lerp(const Vector3<K> &other, const J &progression) const {
	auto ta = *this * (1 - progression);
	auto tb = other * progression;
	return ta + tb;
}

/*template<typename T>
template<typename K>
auto Vector3<T>::Rotate(const Vector3<K> &angle) const
{
	auto matrix = Matrix4::TransformationMatrix(Zero, angle, One);
	return matrix.Transform(Vector4f(*this));
}*/

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Scale(const K &scalar) const {
	return Vector3<decltype(x * scalar)>(x * scalar, y * scalar, z * scalar);
}

template<typename T>
auto Vector3<T>::Normalize() const {
	auto l = Length();

	if (l == 0) {
		throw std::runtime_error("Can't normalize a zero length vector");
	}

	return *this / l;
}

template<typename T>
constexpr auto Vector3<T>::LengthSquared() const {
	return x * x + y * y + z * z;
}

template<typename T>
auto Vector3<T>::Length() const {
	return std::sqrt(LengthSquared());
}

template<typename T>
auto Vector3<T>::Abs() const {
	return Vector2<T>(std::abs(x), std::abs(y), std::abs(z));
}

template<typename T>
constexpr auto Vector3<T>::Min() const {
	return std::min({x, y, z});
}

template<typename T>
constexpr auto Vector3<T>::Max() const {
	return std::max({x, y, z});
}

template<typename T>
constexpr auto Vector3<T>::MinMax() const {
	return std::minmax({x, y, z});
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Min(const Vector3<K> &other) {
	using THighestP = decltype(x + other.x);
	return Vector3<THighestP>(std::min<THighestP>(x, other.x), std::min<THighestP>(y, other.y), std::min<THighestP>(z, other.z));
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Max(const Vector3<K> &other) {
	using THighestP = decltype(x + other.x);
	return Vector3<THighestP>(std::max<THighestP>(x, other.x), std::max<THighestP>(y, other.y), std::max<THighestP>(z, other.z));
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::DistanceSquared(const Vector3<K> &other) const {
	auto dx = x - other.x;
	auto dy = y - other.y;
	auto dz = z - other.z;
	return dx * dx + dy * dy + dz * dz;
}

template<typename T>
template<typename K>
auto Vector3<T>::Distance(const Vector3<K> &other) const {
	return std::sqrt(DistanceSquared(other));
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::DistanceVector(const Vector3<K> &other) const {
	return (*this - other) * (*this - other);
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector3<T>::SmoothDamp(const Vector3<K> &target, const Vector3<J> &rate) const {
	return Maths::SmoothDamp(*this, target, rate);
}

template<typename T>
auto Vector3<T>::CartesianToPolar() const {
	auto radius = std::sqrt(x * x + y * y + z * z);
	auto theta = std::atan2(y, x);
	auto phi = std::atan2(std::sqrt(x * x + y * y), z);
	return Vector3<decltype(radius)>(radius, theta, phi);
}

template<typename T>
auto Vector3<T>::PolarToCartesian() const {
	auto x1 = x * std::sin(z) * std::cos(y);
	auto y1 = x * std::sin(z) * std::sin(y);
	auto z1 = x * std::cos(z);
	return Vector3<decltype(x1)>(x1, y1, z1);
}

template<typename T>
constexpr const T &Vector3<T>::operator[](uint32_t index) const {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::runtime_error("Vector3 index out of bounds!");
	}
}

template<typename T>
constexpr T &Vector3<T>::operator[](uint32_t index) {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::runtime_error("Vector3 index out of bounds!");
	}
}

template<typename T>
template<typename K>
constexpr bool Vector3<T>::operator==(const Vector3<K> &other) const {
	return x == other.x && y == other.y && z == other.z;
}

template<typename T>
template<typename K>
constexpr bool Vector3<T>::operator!=(const Vector3<K> &other) const {
	return !operator==(other);
}

template<typename T>
template<typename U>
constexpr auto Vector3<T>::operator-() const -> std::enable_if_t<std::is_signed_v<U>, Vector3<T>> {
	return {-x, -y, -z};
}

template<typename T>
template<typename U>
constexpr auto Vector3<T>::operator~() const -> std::enable_if_t<std::is_integral_v<U>, Vector3<T>> {
	return {~x, ~y, ~z};
}

template<typename T>
template<typename K>
constexpr Vector3<T> &Vector3<T>::operator+=(const Vector3<K> &other) {
	return *this = Add(other);
}

template<typename T>
template<typename K>
constexpr Vector3<T> &Vector3<T>::operator-=(const Vector3<K> &other) {
	return *this = Subtract(other);
}

template<typename T>
template<typename K>
constexpr Vector3<T> &Vector3<T>::operator*=(const Vector3<K> &other) {
	return *this = Multiply(other);
}

template<typename T>
template<typename K>
constexpr Vector3<T> &Vector3<T>::operator/=(const Vector3<K> &other) {
	return *this = Divide(other);
}

template<typename T>
constexpr Vector3<T> &Vector3<T>::operator+=(const T &other) {
	return *this = Add(Vector3<T>(other));
}

template<typename T>
constexpr Vector3<T> &Vector3<T>::operator-=(const T &other) {
	return *this = Subtract(Vector3<T>(other));
}

template<typename T>
constexpr Vector3<T> &Vector3<T>::operator*=(const T &other) {
	return *this = Multiply(Vector3<T>(other));
}

template<typename T>
constexpr Vector3<T> &Vector3<T>::operator/=(const T &other) {
	return *this = Divide(Vector3<T>(other));
}

template<typename K>
const Node &operator>>(const Node &node, Vector3<K> &vector) {
	node["x"].Get(vector.x);
	node["y"].Get(vector.y);
	node["z"].Get(vector.z);
	return node;
}

template<typename K>
Node &operator<<(Node &node, const Vector3<K> &vector) {
	node["x"].Set(vector.x);
	node["y"].Set(vector.y);
	node["z"].Set(vector.z);
	return node;
}

template<typename K>
std::ostream &operator<<(std::ostream &stream, const Vector3<K> &vector) {
	return stream << vector.x << ", " << vector.y << ", " << vector.z;
}

template<typename K, typename J>
constexpr auto operator+(const Vector3<K> &left, const Vector3<J> &right) {
	return left.Add(right);
}

template<typename K, typename J>
constexpr auto operator-(const Vector3<K> &left, const Vector3<J> &right) {
	return left.Subtract(right);
}

template<typename K, typename J>
constexpr auto operator*(const Vector3<K> &left, const Vector3<J> &right) {
	return left.Multiply(right);
}

template<typename K, typename J>
constexpr auto operator/(const Vector3<K> &left, const Vector3<J> &right) {
	return left.Divide(right);
}

template<typename K, typename J>
constexpr auto operator+(const K &left, const Vector3<J> &right) {
	return Vector3<K>(left).Add(right);
}

template<typename K, typename J>
constexpr auto operator-(const K &left, const Vector3<J> &right) {
	return Vector3<K>(left).Subtract(right);
}

template<typename K, typename J>
constexpr auto operator*(const K &left, const Vector3<J> &right) {
	return Vector3<K>(left).Multiply(right);
}

template<typename K, typename J>
constexpr auto operator/(const K &left, const Vector3<J> &right) {
	return Vector3<K>(left).Divide(right);
}

template<typename K, typename J>
constexpr auto operator+(const Vector3<K> &left, const J &right) {
	return left.Add(Vector3<J>(right));
}

template<typename K, typename J>
constexpr auto operator-(const Vector3<K> &left, const J &right) {
	return left.Subtract(Vector3<J>(right));
}

template<typename K, typename J>
constexpr auto operator*(const Vector3<K> &left, const J &right) {
	return left.Multiply(Vector3<J>(right));
}

template<typename K, typename J>
constexpr auto operator/(const Vector3<K> &left, const J &right) {
	return left.Divide(Vector3<J>(right));
}

template<typename K, typename J>
constexpr auto operator&(const Vector3<K> &left, const Vector3<J> &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector3<J>> {
	return {left.x & right.x, left.y & right.y, left.z & right.z};
}

template<typename K, typename J>
constexpr auto operator|(const Vector3<K> &left, const Vector3<J> &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector3<J>> {
	return {left.x | right.x, left.y | right.y, left.z | right.z};
}

template<typename K, typename J>
constexpr auto operator>>(const Vector3<K> &left, const Vector3<J> &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector3<J>> {
	return {left.x >> right.x, left.y >> right.y, left.z >> right.z};
}

template<typename K, typename J>
constexpr auto operator<<(const Vector3<K> &left, const Vector3<J> &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector3<J>> {
	return {left.x << right.x, left.y << right.y, left.z << right.z};
}

template<typename K, typename J>
constexpr auto operator&(const Vector3<K> &left, const J &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector3<J>> {
	return {left.x & right, left.y & right, left.z & right};
}

template<typename K, typename J>
constexpr auto operator|(const Vector3<K> &left, const J &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector3<J>> {
	return {left.x | right, left.y | right, left.z | right};
}

template<typename K, typename J>
constexpr auto operator>>(const Vector3<K> &left, const J &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector3<J>> {
	return {left.x >> right, left.y >> right, left.z >> right};
}

template<typename K, typename J>
constexpr auto operator<<(const Vector3<K> &left, const J &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector3<J>> {
	return {left.x << right, left.y << right, left.z << right};
}
}

namespace std {
template<typename T>
struct hash<acid::Vector3<T>> {
	size_t operator()(const acid::Vector3<T> &vector) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.x);
		acid::Maths::HashCombine(seed, vector.y);
		acid::Maths::HashCombine(seed, vector.z);
		return seed;
	}
};
}
