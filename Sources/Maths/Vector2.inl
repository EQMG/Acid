#pragma once

#include "Files/Node.hpp"
#include "Maths.hpp"
#include "Vector2.hpp"

namespace acid {
template<typename T>
constexpr Vector2<T>::Vector2(const T &a):
	m_x(a),
	m_y(a) {
}

template<typename T>
constexpr Vector2<T>::Vector2(const T &x, const T &y):
	m_x(x),
	m_y(y) {
}

template<typename T>
template<typename K, typename J>
constexpr Vector2<T>::Vector2(const K &x, const J &y) :
	m_x(static_cast<T>(x)),
	m_y(static_cast<T>(y)) {
}

template<typename T>
template<typename K>
constexpr Vector2<T>::Vector2(const Vector2<K> &source) :
	m_x(static_cast<T>(source.m_x)),
	m_y(static_cast<T>(source.m_y)) {
}

template<typename T>
template<typename K>
constexpr Vector2<T>::Vector2(const Vector3<K> &source):
	m_x(static_cast<T>(source.m_x)),
	m_y(static_cast<T>(source.m_y)) {
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Add(const Vector2<K> &other) const {
	return Vector2<decltype(m_x + other.m_x)>(m_x + other.m_x, m_y + other.m_y);
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Subtract(const Vector2<K> &other) const {
	return Vector2<decltype(m_x - other.m_x)>(m_x - other.m_x, m_y - other.m_y);
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Multiply(const Vector2<K> &other) const {
	return Vector2<decltype(m_x * other.m_x)>(m_x * other.m_x, m_y * other.m_y);
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Divide(const Vector2<K> &other) const {
	return Vector2<decltype(m_x / other.m_x)>(m_x / other.m_x, m_y / other.m_y);
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
	return m_x * other.m_x + m_y * other.m_y;
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
	return Vector2<decltype(m_x * scalar)>(m_x * scalar, m_y * scalar);
}

template<typename T>
template<typename K>
auto Vector2<T>::Rotate(const K &angle) const {
	return Vector2<decltype(m_x * angle)>(m_x * std::cos(angle) - m_y * std::sin(angle), m_x * std::sin(angle) + m_y * std::cos(angle));
}

template<typename T>
template<typename K, typename J>
auto Vector2<T>::Rotate(const K &angle, const Vector2<J> &rotationAxis) const {
	auto x = ((m_x - rotationAxis.m_x) * std::cos(angle)) - ((m_y - rotationAxis.m_y) * std::sin(angle) + rotationAxis.m_x);
	auto y = ((m_x - rotationAxis.m_x) * std::sin(angle)) + ((m_y - rotationAxis.m_y) * std::cos(angle) + rotationAxis.m_y);
	return Vector2<decltype(x)>(x, y);
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
	return m_x * m_x + m_y * m_y;
}

template<typename T>
auto Vector2<T>::Length() const {
	return std::sqrt(LengthSquared());
}

template<typename T>
auto Vector2<T>::Abs() const {
	return Vector2<T>(std::abs(m_x), std::abs(m_y));
}

template<typename T>
constexpr auto Vector2<T>::Min() const {
	return std::min({m_x, m_y});
}

template<typename T>
constexpr auto Vector2<T>::Max() const {
	return std::max({m_x, m_y});
}

template<typename T>
constexpr auto Vector2<T>::MinMax() const {
	return std::minmax({m_x, m_y});
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Min(const Vector2<K> &other) {
	return Vector2<decltype(std::min(m_x, other.m_x))>(std::min(m_x, other.m_x), std::min(m_y, other.m_y));
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::Max(const Vector2<K> &other) {
	return Vector2<decltype(std::max(m_x, other.m_x))>(std::max(m_x, other.m_x), std::max(m_y, other.m_y));
}

template<typename T>
template<typename K>
constexpr auto Vector2<T>::DistanceSquared(const Vector2<K> &other) const {
	auto dx = m_x - other.m_x;
	auto dy = m_y - other.m_y;
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
	auto b1 = ((m_x - v2.m_x) * (v1.m_y - v2.m_y) - (v1.m_x - v2.m_x) * (m_y - v2.m_y)) < 0;
	auto b2 = ((m_x - v3.m_x) * (v2.m_y - v3.m_y) - (v2.m_x - v3.m_x) * (m_y - v3.m_y)) < 0;
	auto b3 = ((m_x - v1.m_x) * (v3.m_y - v1.m_y) - (v3.m_x - v1.m_x) * (m_y - v1.m_y)) < 0;
	return ((b1 == b2) & (b2 == b3));
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector2<T>::SmoothDamp(const Vector2<K> &target, const Vector2<J> &rate) const {
	return Maths::SmoothDamp(*this, target, rate);
}

template<typename T>
auto Vector2<T>::CartesianToPolar() const {
	auto radius = std::sqrt(m_x * m_x + m_y * m_y);
	auto theta = std::atan2(m_y, m_x);
	return Vector2<decltype(radius)>(radius, theta);
}

template<typename T>
auto Vector2<T>::PolarToCartesian() const {
	auto x = m_x * std::cos(m_y);
	auto y = m_x * std::sin(m_y);
	return Vector2<decltype(x)>(x, y);
}

template<typename T>
constexpr const T &Vector2<T>::operator[](uint32_t index) const {
	switch (index) {
	case 0:
		return m_x;
	case 1:
		return m_y;
	default:
		throw std::runtime_error("Vector2 index out of bounds!");
	}
}

template<typename T>
constexpr T &Vector2<T>::operator[](uint32_t index) {
	switch (index) {
	case 0:
		return m_x;
	case 1:
		return m_y;
	default:
		throw std::runtime_error("Vector2 index out of bounds!");
	}
}

template<typename T>
template<typename K>
constexpr bool Vector2<T>::operator==(const Vector2<K> &other) const {
	return m_x == other.m_x && m_y == other.m_y;
}

template<typename T>
template<typename K>
constexpr bool Vector2<T>::operator!=(const Vector2<K> &other) const {
	return !(*this == other);
}

template<typename T>
template<typename U>
constexpr std::enable_if_t<std::is_signed_v<U>, Vector2<T>> Vector2<T>::operator-() const {
	return {-m_x, -m_y};
}

template<typename T>
template<typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Vector2<T>> Vector2<T>::operator~() const {
	return {~m_x, ~m_y};
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
	node["x"].Get(vector.m_x);
	node["y"].Get(vector.m_y);
	return node;
}

template<typename K>
Node &operator<<(Node &node, const Vector2<K> &vector) {
	node["x"].Set(vector.m_x);
	node["y"].Set(vector.m_y);
	return node;
}

template<typename K>
std::ostream &operator<<(std::ostream &stream, const Vector2<K> &vector) {
	return stream << vector.m_x << ", " << vector.m_y;
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
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector2<J>> operator&(const Vector2<K> &left, const Vector2<J> &right) {
	return {left.m_x & right.m_x, left.m_y & right.m_y};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector2<J>> operator|(const Vector2<K> &left, const Vector2<J> &right) {
	return {left.m_x | right.m_x, left.m_y | right.m_y};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector2<J>> operator>>(const Vector2<K> &left, const Vector2<J> &right) {
	return {left.m_x >> right.m_x, left.m_y >> right.m_y};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector2<J>> operator<<(const Vector2<K> &left, const Vector2<J> &right) {
	return {left.m_x << right.m_x, left.m_y << right.m_y};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector2<J>> operator&(const Vector2<K> &left, const J &right) {
	return {left.m_x & right, left.m_y & right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector2<J>> operator|(const Vector2<K> &left, const J &right) {
	return {left.m_x | right, left.m_y | right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector2<J>> operator>>(const Vector2<K> &left, const J &right) {
	return {left.m_x >> right, left.m_y >> right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector2<J>> operator<<(const Vector2<K> &left, const J &right) {
	return {left.m_x << right, left.m_y << right};
}

}

namespace std {
template<typename T>
struct hash<acid::Vector2<T>> {
	size_t operator()(const acid::Vector2<T> &vector) const {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.m_x);
		acid::Maths::HashCombine(seed, vector.m_y);
		return seed;
	}
};
}
