#pragma once

#include "Files/Node.hpp"
#include "Maths.hpp"
#include "Vector2.hpp"

namespace acid {
template<typename T>
constexpr Vector2<T>::Vector2(const T &a):
	x(a),
	y(a) {
}

template<typename T>
constexpr Vector2<T>::Vector2(const T &x, const T &y):
	x(x),
	y(y) {
}

template<typename T>
template<typename K, typename J>
constexpr Vector2<T>::Vector2(const K &x, const J &y) :
	x(static_cast<T>(x)),
	y(static_cast<T>(y)) {
}

template<typename T>
template<typename K>
constexpr Vector2<T>::Vector2(const Vector2<K> &source) :
	x(static_cast<T>(source.x)),
	y(static_cast<T>(source.y)) {
}

template<typename T>
template<typename K>
constexpr Vector2<T>::Vector2(const Vector3<K> &source):
	x(static_cast<T>(source.x)),
	y(static_cast<T>(source.y)) {
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Add(const Vector2<K> &other) const {
	return Vector2<decltype(x + other.x)>(x + other.x, y + other.y);
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Subtract(const Vector2<K> &other) const {
	return Vector2<decltype(x - other.x)>(x - other.x, y - other.y);
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Multiply(const Vector2<K> &other) const {
	return Vector2<decltype(x * other.x)>(x * other.x, y * other.y);
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Divide(const Vector2<K> &other) const {
	return Vector2<decltype(x / other.x)>(x / other.x, y / other.y);
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Angle(const Vector2<K> &other) const {
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
constexpr auto Vector2<T>::Dot(const Vector2<K> &other) const {
	return x * other.x + y * other.y;
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector2<T>::Lerp(const Vector2<K> &other, const J &progression) const {
	auto ta = *this * (1 - progression);
	auto tb = other * progression;
	return ta + tb;
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Scale(const K &scalar) const {
	return Vector2<decltype(x * scalar)>(x * scalar, y * scalar);
}

template<typename T>
template<typename K>
auto Vector2<T>::Rotate(const K &angle) const {
	return Vector2<decltype(x * angle)>(x * std::cos(angle) - y * std::sin(angle), x * std::sin(angle) + y * std::cos(angle));
}

template<typename T>
template<typename K, typename J>
auto Vector2<T>::Rotate(const K &angle, const Vector2<J> &rotationAxis) const {
	auto x1 = ((x - rotationAxis.x) * std::cos(angle)) - ((y - rotationAxis.y) * std::sin(angle) + rotationAxis.x);
	auto y1 = ((x - rotationAxis.x) * std::sin(angle)) + ((y - rotationAxis.y) * std::cos(angle) + rotationAxis.y);
	return Vector2<decltype(x1)>(x1, y1);
}

template<typename T>
auto Vector2<T>::Normalize() const {
	auto l = Length();

	if (l == 0) {
		throw std::runtime_error("Can't normalize a zero length vector");
	}

	return *this / l;
}

template<typename T>
constexpr auto Vector2<T>::LengthSquared() const {
	return x * x + y * y;
}

template<typename T>
auto Vector2<T>::Length() const {
	return std::sqrt(LengthSquared());
}

template<typename T>
auto Vector2<T>::Abs() const {
	return Vector2<T>(std::abs(x), std::abs(y));
}

template<typename T>
constexpr auto Vector2<T>::Min() const {
	return std::min({x, y});
}

template<typename T>
constexpr auto Vector2<T>::Max() const {
	return std::max({x, y});
}

template<typename T>
constexpr auto Vector2<T>::MinMax() const {
	return std::minmax({x, y});
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Min(const Vector2<K> &other) {
	using THighestP = decltype(x + other.x);
	return Vector2<THighestP>(std::min<THighestP>(x, other.x), std::min<THighestP>(y, other.y));
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Max(const Vector2<K> &other) {
	using THighestP = decltype(x + other.x);
	return Vector2<THighestP>(std::max<THighestP>(x, other.x), std::max<THighestP>(y, other.y));
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::DistanceSquared(const Vector2<K> &other) const {
	auto dx = x - other.x;
	auto dy = y - other.y;
	return dx * dx + dy * dy;
}

template<typename T>
template<typename K>
auto Vector2<T>::Distance(const Vector2<K> &other) const {
	return std::sqrt(DistanceSquared(other));
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::DistanceVector(const Vector2<K> &other) const {
	return (*this - other) * (*this - other);
}

template<typename T>
template<typename K>
constexpr bool Vector2<T>::InTriangle(const Vector2<K> &v1, const Vector2<K> &v2, const Vector2<K> &v3) const {
	auto b1 = ((x - v2.x) * (v1.y - v2.y) - (v1.x - v2.x) * (y - v2.y)) < 0;
	auto b2 = ((x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (y - v3.y)) < 0;
	auto b3 = ((x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (y - v1.y)) < 0;
	return ((b1 == b2) & (b2 == b3));
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector2<T>::SmoothDamp(const Vector2<K> &target, const Vector2<J> &rate) const {
	return Maths::SmoothDamp(*this, target, rate);
}

template<typename T>
auto Vector2<T>::CartesianToPolar() const {
	auto radius = std::sqrt(x * x + y * y);
	auto theta = std::atan2(y, x);
	return Vector2<decltype(radius)>(radius, theta);
}

template<typename T>
auto Vector2<T>::PolarToCartesian() const {
	auto x1 = x * std::cos(y);
	auto y1 = x * std::sin(y);
	return Vector2<decltype(x1)>(x1, y1);
}

template<typename T>
constexpr const T &Vector2<T>::operator[](uint32_t index) const {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::runtime_error("Vector2 index out of bounds!");
	}
}

template<typename T>
constexpr T &Vector2<T>::operator[](uint32_t index) {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::runtime_error("Vector2 index out of bounds!");
	}
}

template<typename T>
template<typename K>
constexpr bool Vector2<T>::operator==(const Vector2<K> &other) const {
	return x == other.x && y == other.y;
}

template<typename T>
template<typename K>
constexpr bool Vector2<T>::operator!=(const Vector2<K> &other) const {
	return !operator==(other);
}

template<typename T>
template<typename U>
constexpr auto Vector2<T>::operator-() const -> std::enable_if_t<std::is_signed_v<U>, Vector2<T>> {
	return {-x, -y};
}

template<typename T>
template<typename U>
constexpr auto Vector2<T>::operator~() const -> std::enable_if_t<std::is_integral_v<U>, Vector2<T>> {
	return {~x, ~y};
}

template<typename T>
template<typename K>
constexpr Vector2<T> &Vector2<T>::operator+=(const Vector2<K> &other) {
	return *this = Add(other);
}

template<typename T>
template<typename K>
constexpr Vector2<T> &Vector2<T>::operator-=(const Vector2<K> &other) {
	return *this = Subtract(other);
}

template<typename T>
template<typename K>
constexpr Vector2<T> &Vector2<T>::operator*=(const Vector2<K> &other) {
	return *this = Multiply(other);
}

template<typename T>
template<typename K>
constexpr Vector2<T> &Vector2<T>::operator/=(const Vector2<K> &other) {
	return *this = Divide(other);
}

template<typename T>
constexpr Vector2<T> &Vector2<T>::operator+=(const T &other) {
	return *this = Add(Vector2<T>(other));
}

template<typename T>
constexpr Vector2<T> &Vector2<T>::operator-=(const T &other) {
	return *this = Subtract(Vector2<T>(other));
}

template<typename T>
constexpr Vector2<T> &Vector2<T>::operator*=(const T &other) {
	return *this = Multiply(Vector2<T>(other));
}

template<typename T>
constexpr Vector2<T> &Vector2<T>::operator/=(const T &other) {
	return *this = Divide(Vector2<T>(other));
}

template<typename K>
const Node &operator>>(const Node &node, Vector2<K> &vector) {
	node["x"].Get(vector.x);
	node["y"].Get(vector.y);
	return node;
}

template<typename K>
Node &operator<<(Node &node, const Vector2<K> &vector) {
	node["x"].Set(vector.x);
	node["y"].Set(vector.y);
	return node;
}

template<typename K>
std::ostream &operator<<(std::ostream &stream, const Vector2<K> &vector) {
	return stream << vector.x << ", " << vector.y;
}

template<typename K, typename J>
constexpr auto operator+(const Vector2<K> &left, const Vector2<J> &right) {
	return left.Add(right);
}

template<typename K, typename J>
constexpr auto operator-(const Vector2<K> &left, const Vector2<J> &right) {
	return left.Subtract(right);
}

template<typename K, typename J>
constexpr auto operator*(const Vector2<K> &left, const Vector2<J> &right) {
	return left.Multiply(right);
}

template<typename K, typename J>
constexpr auto operator/(const Vector2<K> &left, const Vector2<J> &right) {
	return left.Divide(right);
}

template<typename K, typename J>
constexpr auto operator+(const K &left, const Vector2<J> &right) {
	return Vector2<K>(left).Add(right);
}

template<typename K, typename J>
constexpr auto operator-(const K &left, const Vector2<J> &right) {
	return Vector2<K>(left).Subtract(right);
}

template<typename K, typename J>
constexpr auto operator*(const K &left, const Vector2<J> &right) {
	return Vector2<K>(left).Multiply(right);
}

template<typename K, typename J>
constexpr auto operator/(const K &left, const Vector2<J> &right) {
	return Vector2<K>(left).Divide(right);
}

template<typename K, typename J>
constexpr auto operator+(const Vector2<K> &left, const J &right) {
	return left.Add(Vector2<J>(right));
}

template<typename K, typename J>
constexpr auto operator-(const Vector2<K> &left, const J &right) {
	return left.Subtract(Vector2<J>(right));
}

template<typename K, typename J>
constexpr auto operator*(const Vector2<K> &left, const J &right) {
	return left.Multiply(Vector2<J>(right));
}

template<typename K, typename J>
constexpr auto operator/(const Vector2<K> &left, const J &right) {
	return left.Divide(Vector2<J>(right));
}

template<typename K, typename J>
constexpr auto operator&(const Vector2<K> &left, const Vector2<J> &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector2<J>> {
	return {left.x & right.x, left.y & right.y};
}

template<typename K, typename J>
constexpr auto operator|(const Vector2<K> &left, const Vector2<J> &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector2<J>> {
	return {left.x | right.x, left.y | right.y};
}

template<typename K, typename J>
constexpr auto operator>>(const Vector2<K> &left, const Vector2<J> &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector2<J>> {
	return {left.x >> right.x, left.y >> right.y};
}

template<typename K, typename J>
constexpr auto operator<<(const Vector2<K> &left, const Vector2<J> &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector2<J>> {
	return {left.x << right.x, left.y << right.y};
}

template<typename K, typename J>
constexpr auto operator&(const Vector2<K> &left, const J &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector2<J>> {
	return {left.x & right, left.y & right};
}

template<typename K, typename J>
constexpr auto operator|(const Vector2<K> &left, const J &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector2<J>> {
	return {left.x | right, left.y | right};
}

template<typename K, typename J>
constexpr auto operator>>(const Vector2<K> &left, const J &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector2<J>> {
	return {left.x >> right, left.y >> right};
}

template<typename K, typename J>
constexpr auto operator<<(const Vector2<K> &left, const J &right) -> std::enable_if_t<std::is_integral_v<K> &&std::is_integral_v<J>, Vector2<J>> {
	return {left.x << right, left.y << right};
}
}

namespace std {
template<typename T>
struct hash<acid::Vector2<T>> {
	size_t operator()(const acid::Vector2<T> &vector) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.x);
		acid::Maths::HashCombine(seed, vector.y);
		return seed;
	}
};
}
