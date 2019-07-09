#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"
#include "Vector3.hpp"

namespace acid
{
template<typename T>
constexpr Vector3<T>::Vector3(const T &a):
	m_x{a},
	m_y{a},
	m_z{a}
{
}

template<typename T>
constexpr Vector3<T>::Vector3(const T &x, const T &y, const T &z):
	m_x{x},
	m_y{y},
	m_z{z}
{
}

template<typename T>
template<typename K, typename J, typename H>
constexpr Vector3<T>::Vector3(const K &x, const J &y, const H &z) :
	m_x{static_cast<T>(x)},
	m_y{static_cast<T>(y)},
	m_z{static_cast<T>(z)}
{
}

template<typename T>
template<typename K, typename J>
constexpr Vector3<T>::Vector3(const Vector2<K> &source, const J &z):
	m_x{static_cast<T>(source.m_x)},
	m_y{static_cast<T>(source.m_y)},
	m_z{static_cast<T>(z)}
{
}

template<typename T>
template<typename K>
constexpr Vector3<T>::Vector3(const Vector3<K> &source):
	m_x{static_cast<T>(source.m_x)},
	m_y{static_cast<T>(source.m_y)},
	m_z{static_cast<T>(source.m_z)}
{
}

template<typename T>
template<typename K>
constexpr Vector3<T>::Vector3(const Vector4<K> &source):
	m_x{static_cast<T>(source.m_x)},
	m_y{static_cast<T>(source.m_y)},
	m_z{static_cast<T>(source.m_z)}
{
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Add(const Vector3<K> &other) const
{
	return Vector3<decltype(m_x + other.m_x)>{m_x + other.m_x, m_y + other.m_y, m_z + other.m_z};
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Subtract(const Vector3<K> &other) const
{
	return Vector3<decltype(m_x - other.m_x)>{m_x - other.m_x, m_y - other.m_y, m_z - other.m_z};
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Multiply(const Vector3<K> &other) const
{
	return Vector3<decltype(m_x * other.m_x)>{m_x * other.m_x, m_y * other.m_y, m_z * other.m_z};
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Divide(const Vector3<K> &other) const
{
	return Vector3<decltype(m_x / other.m_x)>{m_x / other.m_x, m_y / other.m_y, m_z / other.m_z};
}

template<typename T>
template<typename K>
auto Vector3<T>::Angle(const Vector3<K> &other) const
{
	auto dls{Dot(other) / (Length() * other.Length())};

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
constexpr auto Vector3<T>::Dot(const Vector3<K> &other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Cross(const Vector3<K> &other) const
{
	return Vector3<decltype(m_x * other.m_x)>{m_y *other.m_z - m_z * other.m_y, other.m_x *m_z - other.m_z * m_x, m_x *other.m_y - m_y * other.m_x};
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector3<T>::Lerp(const Vector3<K> &other, const J &progression) const
{
	auto ta{*this * (1 - progression)};
	auto tb{other * progression};
	return ta + tb;
} 

/*template<typename T>
template<typename K>
auto Vector3<T>::Rotate(const Vector3<K> &angle) const
{
	auto matrix{Matrix4::TransformationMatrix(Zero, angle, One)};
	return matrix.Transform(Vector4f{*this});
}*/

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Scale(const K &scalar) const
{
	return Vector3<decltype(m_x * scalar)>{m_x *scalar, m_y *scalar, m_z *scalar};
}

template<typename T>
auto Vector3<T>::Normalize() const
{
	auto l{Length()};

	if (l == 0)
	{
		throw std::runtime_error{"Can't normalize a zero length vector"};
	}

	return *this / l;
}

template<typename T>
constexpr auto Vector3<T>::LengthSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

template<typename T>
auto Vector3<T>::Length() const
{
	return std::sqrt(LengthSquared());
}

template<typename T>
auto Vector3<T>::Abs() const
{
	return Vector2<T>(std::abs(m_x), std::abs(m_y), std::abs(m_z));
}

template<typename T>
constexpr auto Vector3<T>::Min() const
{
	return std::min({m_x, m_y, m_z});
}

template<typename T>
constexpr auto Vector3<T>::Max() const
{
	return std::max({m_x, m_y, m_z});
}

template<typename T>
constexpr auto Vector3<T>::MinMax() const
{
	return std::minmax({m_x, m_y, m_z});
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Min(const Vector3<K> &other)
{
	return Vector3<decltype(std::min(m_x, other.m_x))>{std::min(m_x, other.m_x), std::min(m_y, other.m_y), std::min(m_z, other.m_z)};
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::Max(const Vector3<K> &other)
{
	return Vector3<decltype(std::max(m_x, other.m_x))>{std::max(m_x, other.m_x), std::max(m_y, other.m_y), std::max(m_z, other.m_z)};
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::DistanceSquared(const Vector3<K> &other) const
{
	auto dx{m_x - other.m_x};
	auto dy{m_y - other.m_y};
	auto dz{m_z - other.m_z};
	return dx * dx + dy * dy + dz * dz;
}

template<typename T>
template<typename K>
auto Vector3<T>::Distance(const Vector3<K> &other) const
{
	return std::sqrt(DistanceSquared(other));
}

template<typename T>
template<typename K>
constexpr auto Vector3<T>::DistanceVector(const Vector3<K> &other) const
{
	return (*this - other) * (*this - other);
}

template<typename T>
template<typename K, typename J>
constexpr auto Vector3<T>::SmoothDamp(const Vector3<K> &target, const Vector3<J> &rate) const
{
	return Maths::SmoothDamp(*this, target, rate);
}

template<typename T>
auto Vector3<T>::CartesianToPolar() const
{
	auto radius{std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z)};
	auto theta{std::atan2(m_y, m_x)};
	auto phi{std::atan2(std::sqrt(m_x * m_x + m_y * m_y), m_z)};
	return Vector3<decltype(radius)>{radius, theta, phi};
}

template<typename T>
auto Vector3<T>::PolarToCartesian() const
{
	auto x{m_x * std::sin(m_z) * std::cos(m_y)};
	auto y{m_x * std::sin(m_z) * std::sin(m_y)};
	auto z{m_x * std::cos(m_z)};
	return Vector3<decltype(x)>{x, y, z};
}

template<typename T>
template<typename K>
constexpr bool Vector3<T>::operator==(const Vector3<K> &other) const
{
	return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
}

template<typename T>
template<typename K>
constexpr bool Vector3<T>::operator!=(const Vector3<K> &other) const
{
	return !(*this == other);
}

template<typename T>
template<typename U>
constexpr std::enable_if_t<std::is_signed_v<U>, Vector3<T>> Vector3<T>::operator-() const
{
	return {-m_x, -m_y, -m_z};
}

template<typename T>
template<typename U>
constexpr std::enable_if_t<std::is_integral_v<U>, Vector3<T>> Vector3<T>::operator~() const
{
	return {~m_x, ~m_y, ~m_z};
}

template<typename T>
constexpr const T &Vector3<T>::operator[](const uint32_t &index) const
{
	switch (index)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	default:
		throw std::runtime_error{"Vector3 index out of bounds!"};
	}
}

template<typename T>
constexpr T &Vector3<T>::operator[](const uint32_t &index)
{
	switch (index)
	{
	case 0:
		return m_x;
	case 1:
		return m_y;
	case 2:
		return m_z;
	default:
		throw std::runtime_error{"Vector3 index out of bounds!"};
	}
}

template<typename T>
template<typename K>
constexpr Vector3<T> &Vector3<T>::operator+=(const Vector3<K> &other)
{
	return *this = Add(other);
}

template<typename T>
template<typename K>
constexpr Vector3<T> &Vector3<T>::operator-=(const Vector3<K> &other)
{
	return *this = Subtract(other);
}

template<typename T>
template<typename K>
constexpr Vector3<T> &Vector3<T>::operator*=(const Vector3<K> &other)
{
	return *this = Multiply(other);
}

template<typename T>
template<typename K>
constexpr Vector3<T> &Vector3<T>::operator/=(const Vector3<K> &other)
{
	return *this = Divide(other);
}

template<typename T>
constexpr Vector3<T> &Vector3<T>::operator+=(const T &other)
{
	return *this = Add(Vector3<T>{other});
}

template<typename T>
constexpr Vector3<T> &Vector3<T>::operator-=(const T &other)
{
	return *this = Subtract(Vector3<T>{other});
}

template<typename T>
constexpr Vector3<T> &Vector3<T>::operator*=(const T &other)
{
	return *this = Multiply(Vector3<T>{other});
}

template<typename T>
constexpr Vector3<T> &Vector3<T>::operator/=(const T &other)
{
	return *this = Divide(Vector3<T>{other});
}

template<typename K>
const Metadata &operator>>(const Metadata &metadata, Vector3<K> &vector)
{
	metadata.GetChild("x", vector.m_x);
	metadata.GetChild("y", vector.m_y);
	metadata.GetChild("z", vector.m_z);
	return metadata;
}

template<typename K>
Metadata &operator<<(Metadata &metadata, const Vector3<K> &vector)
{
	metadata.SetChild("x", vector.m_x);
	metadata.SetChild("y", vector.m_y);
	metadata.SetChild("z", vector.m_z);
	return metadata;
}

template<typename K>
std::ostream &operator<<(std::ostream &stream, const Vector3<K> &vector)
{
	return stream << vector.m_x << ", " << vector.m_y << ", " << vector.m_z;
}

template<typename K, typename J>
constexpr auto operator+(const Vector3<K> &left, const Vector3<J> &right)
{
	return left.Add(right);
}

template<typename K, typename J>
constexpr auto operator-(const Vector3<K> &left, const Vector3<J> &right)
{
	return left.Subtract(right);
}

template<typename K, typename J>
constexpr auto operator*(const Vector3<K> &left, const Vector3<J> &right)
{
	return left.Multiply(right);
}

template<typename K, typename J>
constexpr auto operator/(const Vector3<K> &left, const Vector3<J> &right)
{
	return left.Divide(right);
}

template<typename K, typename J>
constexpr auto operator+(const K &left, const Vector3<J> &right)
{
	return Vector3<K>{left}.Add(right);
}

template<typename K, typename J>
constexpr auto operator-(const K &left, const Vector3<J> &right)
{
	return Vector3<K>{left}.Subtract(right);
}

template<typename K, typename J>
constexpr auto operator*(const K &left, const Vector3<J> &right)
{
	return Vector3<K>{left}.Multiply(right);
}

template<typename K, typename J>
constexpr auto operator/(const K &left, const Vector3<J> &right)
{
	return Vector3<K>{left}.Divide(right);
}

template<typename K, typename J>
constexpr auto operator+(const Vector3<K> &left, const J &right)
{
	return left.Add(Vector3<J>{right});
}

template<typename K, typename J>
constexpr auto operator-(const Vector3<K> &left, const J &right)
{
	return left.Subtract(Vector3<J>{right});
}

template<typename K, typename J>
constexpr auto operator*(const Vector3<K> &left, const J &right)
{
	return left.Multiply(Vector3<J>{right});
}

template<typename K, typename J>
constexpr auto operator/(const Vector3<K> &left, const J &right)
{
	return left.Divide(Vector3<J>{right});
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector3<J>> operator&(const Vector3<K> &left, const Vector3<J> &right)
{
	return {left.m_x & right.m_x, left.m_y & right.m_y, left.m_z & right.m_z};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector3<J>> operator|(const Vector3<K> &left, const Vector3<J> &right)
{
	return {left.m_x | right.m_x, left.m_y | right.m_y, left.m_z | right.m_z};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector3<J>> operator>>(const Vector3<K> &left, const Vector3<J> &right)
{
	return {left.m_x >> right.m_x, left.m_y >> right.m_y, left.m_z >> right.m_z};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector3<J>> operator<<(const Vector3<K> &left, const Vector3<J> &right)
{
	return {left.m_x << right.m_x, left.m_y << right.m_y, left.m_z << right.m_z};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector3<J>> operator&(const Vector3<K> &left, const J &right)
{
	return {left.m_x & right, left.m_y & right, left.m_z & right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector3<J>> operator|(const Vector3<K> &left, const J &right)
{
	return {left.m_x | right, left.m_y | right, left.m_z | right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector3<J>> operator>>(const Vector3<K> &left, const J &right)
{
	return {left.m_x >> right, left.m_y >> right, left.m_z >> right};
}

template<typename K, typename J>
constexpr std::enable_if_t<std::is_integral_v<K> && std::is_integral_v<J>, Vector3<J>> operator<<(const Vector3<K> &left, const J &right)
{
	return {left.m_x << right, left.m_y << right, left.m_z << right};
}
}

namespace std
{
template<typename T>
struct hash<acid::Vector3<T>>
{
	size_t operator()(const acid::Vector3<T> &vector) const
	{
		size_t seed{};
		acid::Maths::HashCombine(seed, vector.m_x);
		acid::Maths::HashCombine(seed, vector.m_y);
		acid::Maths::HashCombine(seed, vector.m_z);
		return seed;
	}
};
}
