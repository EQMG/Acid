#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"
#include "Vector4.hpp"

namespace acid
{
template<typename T>
Vector4<T>::Vector4():
	m_x(0),
	m_y(0),
	m_z(0),
	m_w(1)
{
}

template<typename T>
Vector4<T>::Vector4(const T &a):
	m_x(a),
	m_y(a),
	m_z(a),
	m_w(a)
{
}

template<typename T>
Vector4<T>::Vector4(const T &x, const T &y, const T &z, const T &w):
	m_x(x),
	m_y(y),
	m_z(z),
	m_w(w)
{
}

template<typename T>
template<typename K>
Vector4<T>::Vector4(const Vector2<K> &left, const Vector2<K> &right):
	m_x(static_cast<T>(left.m_x)),
	m_y(static_cast<T>(left.m_y)),
	m_z(static_cast<T>(right.m_x)),
	m_w(static_cast<T>(right.m_y))
{
}

template<typename T>
template<typename K>
Vector4<T>::Vector4(const Vector3<K> &source, const T &w):
	m_x(static_cast<T>(source.m_x)),
	m_y(static_cast<T>(source.m_y)),
	m_z(static_cast<T>(source.m_z)),
	m_w(w)
{
}

template<typename T>
template<typename K>
Vector4<T>::Vector4(const Vector4<K> &source):
	m_x(static_cast<T>(source.m_x)),
	m_y(static_cast<T>(source.m_y)),
	m_z(static_cast<T>(source.m_z)),
	m_w(static_cast<T>(source.m_w))
{
}

template<typename T>
template<typename K>
auto Vector4<T>::Add(const Vector4<K> &other) const
{
	return Vector4<decltype(m_x + other.m_x)>(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

template<typename T>
template<typename K>
auto Vector4<T>::Subtract(const Vector4<K> &other) const
{
	return Vector4<decltype(m_x - other.m_x)>(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
}

template<typename T>
template<typename K>
auto Vector4<T>::Multiply(const Vector4<K> &other) const
{
	return Vector4<decltype(m_x * other.m_x)>(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z, m_w * other.m_w);
}

template<typename T>
template<typename K>
auto Vector4<T>::Divide(const Vector4<K> &other) const
{
	return Vector4<decltype(m_x / other.m_x)>(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z, m_w / other.m_w);
}

template<typename T>
template<typename K>
auto Vector4<T>::Angle(const Vector4<K> &other) const
{
	auto dls = Dot(other) / (Length() * other.Length());

	if (dls < -1)
	{
		dls = -1;
	}
	else if (dls > 1)
	{
		dls = 1;
	}

	return std::acos(dls);
}

template<typename T>
template<typename K>
auto Vector4<T>::Dot(const Vector4<K> &other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
}

template<typename T>
template<typename K, typename J>
auto Vector4<T>::Lerp(const Vector4<K> &other, const J &progression) const
{
	auto ta = *this * (1 - progression);
	auto tb = other * progression;
	return ta + tb;
}

template<typename T>
template<typename K>
auto Vector4<T>::Scale(const K &scalar) const
{
	return Vector4<decltype(m_x * scalar)>(m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar);
}

template<typename T>
auto Vector4<T>::Normalize() const
{
	auto l = Length();
	if (l == 0)
	{
		throw std::runtime_error("Can't normalize a zero length vector");
	}
	return *this / l;
}

template<typename T>
auto Vector4<T>::LengthSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

template<typename T>
auto Vector4<T>::Length() const
{
	return std::sqrt(LengthSquared());
}

template<typename T>
auto Vector4<T>::Abs() const
{
	return Vector2<T>(std::abs(m_x), std::abs(m_y), std::abs(m_z), std::abs(m_w));
}

template<typename T>
auto Vector4<T>::Min() const
{
	return std::min({m_x, m_y, m_z, m_w});
}

template<typename T>
auto Vector4<T>::Max() const
{
	return std::max({m_x, m_y, m_z, m_w});
}

template<typename T>
auto Vector4<T>::MinMax() const
{
	return std::minmax({m_x, m_y, m_z, m_w});
}

template<typename T>
template<typename K>
auto Vector4<T>::Min(const Vector4<K> &other)
{
	return Vector4<decltype(std::min(m_x, other.m_x))>(std::min(m_x, other.m_x), std::min(m_y, other.m_y), std::min(m_z, other.m_z), std::min(m_w, other.m_w));
}

template<typename T>
template<typename K>
auto Vector4<T>::Max(const Vector4<K> &other)
{
	return Vector4<decltype(std::max(m_x, other.m_x))>(std::max(m_x, other.m_x), std::max(m_y, other.m_y), std::max(m_z, other.m_z), std::max(m_w, other.m_w));
}

template<typename T>
template<typename K>
auto Vector4<T>::DistanceSquared(const Vector4<K> &other) const
{
	auto dx = m_x - other.m_x;
	auto dy = m_y - other.m_y;
	auto dz = m_z - other.m_z;
	auto dw = m_w - other.m_w;
	return dx * dx + dy * dy + dz * dz + dw * dw;
}

template<typename T>
template<typename K>
auto Vector4<T>::Distance(const Vector4<K> &other) const
{
	return std::sqrt(DistanceSquared(other));
}

template<typename T>
template<typename K>
auto Vector4<T>::DistanceVector(const Vector4<K> &other) const
{
	return (*this - other) * (*this - other);
}

template<typename T>
template<typename K, typename J>
auto Vector4<T>::SmoothDamp(const Vector4<K> &target, const Vector4<J> &rate) const
{
	return Maths::SmoothDamp(*this, target, rate);
}

template<typename T>
std::string Vector4<T>::ToString() const
{
	std::stringstream stream;
	stream.precision(10);
	stream << "Vector4(" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << ")";
	return stream.str();
}

template<typename T>
template<typename K>
bool Vector4<T>::operator==(const Vector4<K> &other) const
{
	return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && m_w == other.m_w;
}

template<typename T>
template<typename K>
bool Vector4<T>::operator!=(const Vector4<K> &other) const
{
	return !(*this == other);
}

template<typename T>
template<typename U>
std::enable_if_t<std::is_signed_v<U>, Vector4<T>> Vector4<T>::operator-() const
{
	return Vector4(-m_x, -m_y, -m_z, -m_w);
}

template<typename T>
template<typename U>
std::enable_if_t<std::is_integral_v<U>, Vector4<T>> Vector4<T>::operator~() const
{
	return Vector4(~m_x, ~m_y, ~m_z, ~m_w);
}

template<typename T>
const T &Vector4<T>::operator[](const uint32_t &index) const
{
	switch (index)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	case 3:
		return m_w;
	default:
		throw std::runtime_error("Vector4 index out of bounds!");
	}
}

template<typename T>
T &Vector4<T>::operator[](const uint32_t &index)
{
	switch (index)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	case 3:
		return m_w;
	default:
		throw std::runtime_error("Vector4 index out of bounds!");
	}
}

template<typename T>
template<typename K>
Vector4<T> &Vector4<T>::operator+=(const Vector4<K> &other)
{
	return *this = Add(other);
}

template<typename T>
template<typename K>
Vector4<T> &Vector4<T>::operator-=(const Vector4<K> &other)
{
	return *this = Subtract(other);
}

template<typename T>
template<typename K>
Vector4<T> &Vector4<T>::operator*=(const Vector4<K> &other)
{
	return *this = Multiply(other);
}

template<typename T>
template<typename K>
Vector4<T> &Vector4<T>::operator/=(const Vector4<K> &other)
{
	return *this = Divide(other);
}

template<typename T>
Vector4<T> &Vector4<T>::operator+=(const T &other)
{
	return *this = Add(Vector4<T>(other));
}

template<typename T>
Vector4<T> &Vector4<T>::operator-=(const T &other)
{
	return *this = Subtract(Vector4<T>(other));
}

template<typename T>
Vector4<T> &Vector4<T>::operator*=(const T &other)
{
	return *this = Multiply(Vector4<T>(other));
}

template<typename T>
Vector4<T> &Vector4<T>::operator/=(const T &other)
{
	return *this = Divide(Vector4<T>(other));
}

template<typename K>
const Metadata &operator>>(const Metadata &metadata, Vector4<K> &vector)
{
	metadata.GetChild("x", vector.m_x);
	metadata.GetChild("y", vector.m_y);
	metadata.GetChild("z", vector.m_z);
	metadata.GetChild("w", vector.m_w);
	return metadata;
}

template<typename K>
Metadata &operator<<(Metadata &metadata, const Vector4<K> &vector)
{
	metadata.SetChild("x", vector.m_x);
	metadata.SetChild("y", vector.m_y);
	metadata.SetChild("z", vector.m_z);
	metadata.SetChild("w", vector.m_w);
	return metadata;
}

template<typename K>
std::ostream &operator<<(std::ostream &stream, const Vector4<K> &vector)
{
	stream << vector.ToString();
	return stream;
}

template<typename K, typename J>
auto operator+(const Vector4<K> &left, const Vector4<J> &right)
{
	return left.Add(right);
}

template<typename K, typename J>
auto operator-(const Vector4<K> &left, const Vector4<J> &right)
{
	return left.Subtract(right);
}

template<typename K, typename J>
auto operator*(const Vector4<K> &left, const Vector4<J> &right)
{
	return left.Multiply(right);
}

template<typename K, typename J>
auto operator/(const Vector4<K> &left, const Vector4<J> &right)
{
	return left.Divide(right);
}

template<typename K, typename J>
auto operator+(const K &left, const Vector4<J> &right)
{
	return Vector4<K>(left).Add(right);
}

template<typename K, typename J>
auto operator-(const K &left, const Vector4<J> &right)
{
	return Vector4<K>(left).Subtract(right);
}

template<typename K, typename J>
auto operator*(const K &left, const Vector4<J> &right)
{
	return Vector4<K>(left).Multiply(right);
}

template<typename K, typename J>
auto operator/(const K &left, const Vector4<J> &right)
{
	return Vector4<K>(left).Divide(right);
}

template<typename K, typename J>
auto operator+(const Vector4<K> &left, const J &right)
{
	return left.Add(Vector4<J>(right));
}

template<typename K, typename J>
auto operator-(const Vector4<K> &left, const J &right)
{
	return left.Subtract(Vector4<J>(right));
}

template<typename K, typename J>
auto operator*(const Vector4<K> &left, const J &right)
{
	return left.Multiply(Vector4<J>(right));
}

template<typename K, typename J>
auto operator/(const Vector4<K> &left, const J &right)
{
	return left.Divide(Vector4<J>(right));
}

template<typename K, typename J>
std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator&(const Vector4<K> &left, const Vector4<J> &right)
{
	return Vector4<J>(left.m_x & right.m_x, left.m_y & right.m_y, left.m_z & right.m_z, left.m_w & right.m_w);
}

template<typename K, typename J>
std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator|(const Vector4<K> &left, const Vector4<J> &right)
{
	return Vector4<J>(left.m_x | right.m_x, left.m_y | right.m_y, left.m_z | right.m_z, left.m_w | right.m_w);
}

template<typename K, typename J>
std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator>>(const Vector4<K> &left, const Vector4<J> &right)
{
	return Vector4<J>(left.m_x >> right.m_x, left.m_y >> right.m_y, left.m_z >> right.m_z, left.m_w >> right.m_w);
}

template<typename K, typename J>
std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator<<(const Vector4<K> &left, const Vector4<J> &right)
{
	return Vector4<J>(left.m_x << right.m_x, left.m_y << right.m_y, left.m_z << right.m_z, left.m_w << right.m_w);
}

template<typename K, typename J>
std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator&(const Vector4<K> &left, const J &right)
{
	return Vector4<J>(left.m_x & right, left.m_y & right, left.m_z & right, left.m_w & right);
}

template<typename K, typename J>
std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator|(const Vector4<K> &left, const J &right)
{
	return Vector4<J>(left.m_x | right, left.m_y | right, left.m_z | right, left.m_w | right);
}

template<typename K, typename J>
std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator>>(const Vector4<K> &left, const J &right)
{
	return Vector4<J>(left.m_x >> right, left.m_y >> right, left.m_z >> right, left.m_w >> right);
}

template<typename K, typename J>
std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector4<J>> operator<<(const Vector4<K> &left, const J &right)
{
	return Vector4<J>(left.m_x << right, left.m_y << right, left.m_z << right, left.m_w << right);
}

}

namespace std
{
template<typename T>
struct hash<acid::Vector4<T>>
{
	size_t operator()(const acid::Vector4<T> &vector) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.m_x);
		acid::Maths::HashCombine(seed, vector.m_y);
		acid::Maths::HashCombine(seed, vector.m_z);
		acid::Maths::HashCombine(seed, vector.m_w);
		return seed;
	}
};
}
