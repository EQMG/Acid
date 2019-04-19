#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"

namespace acid
{
template<typename T>
class Vector3;

/**
 * @brief Holds a 2-tuple vector.
 * @tparam T The value type.
 */
template<typename T>
class Vector2
{
public:
	/**
	 * Constructor for Vector2.
	 **/
	Vector2() :
		m_x(0),
		m_y(0)
	{
	}

	/**
	 * Constructor for Vector2.
	 * @param a The value to set all components to.
	 **/
	explicit Vector2(const T &a) :
		m_x(a),
		m_y(a)
	{
	}

	/**
	 * Constructor for Vector2.
	 * @param x Start x. 
	 * @param y Start y. 
	 **/
	Vector2(const T &x, const T &y) :
		m_x(x),
		m_y(y)
	{
	}

	/**
	 * Constructor for Vector2.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	Vector2(const Vector2<K> &source) :
		m_x(static_cast<T>(source.m_x)),
		m_y(static_cast<T>(source.m_y))
	{
	}

	/**
	 * Constructor for Vector3.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	explicit Vector2(const Vector3<K> &source) :
		m_x(static_cast<T>(source.m_x)),
		m_y(static_cast<T>(source.m_y))
	{
	}

	/**
	 * Adds this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	template<typename K>
	auto Add(const Vector2<K> &other) const
	{
		return Vector2<decltype(m_x + other.m_x)>(m_x + other.m_x, m_y + other.m_y);
	}

	/**
	 * Subtracts this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	template<typename K>
	auto Subtract(const Vector2<K> &other) const
	{
		return Vector2<decltype(m_x - other.m_x)>(m_x - other.m_x, m_y - other.m_y);
	}

	/**
	 * Multiplies this vector with another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	template<typename K>
	auto Multiply(const Vector2<K> &other) const
	{
		return Vector2<decltype(m_x * other.m_x)>(m_x * other.m_x, m_y * other.m_y);
	}

	/**
	 * Divides this vector by another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	template<typename K>
	auto Divide(const Vector2<K> &other) const
	{
		return Vector2<decltype(m_x / other.m_x)>(m_x / other.m_x, m_y / other.m_y);
	}

	/**
	 * Calculates the angle between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The angle, in radians. 
	 **/
	template<typename K>
	auto Angle(const Vector2<K> &other) const
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

	/**
	 * Calculates the dot product of the this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The dot product. 
	 **/
	template<typename K>
	auto Dot(const Vector2<K> &other) const
	{
		return m_x * other.m_x + m_y * other.m_y;
	}

	/**
	 * Calculates the linear interpolation between this vector and another vector.
	 * @tparam K The others type.
	 * @tparam J The progression type.
	 * @param other The other quaternion. 
	 * @param progression The progression. 
	 * @return Left lerp right. 
	 **/
	template<typename K, typename J = float>
	auto Lerp(const Vector2<K> &other, const J &progression) const
	{
		auto ta = *this * (1 - progression);
		auto tb = other * progression;
		return ta + tb;
	}

	/**
	 * Scales this vector by a scalar.
	 * @tparam K The scalar type.
	 * @param scalar The scalar value. 
	 * @return The scaled vector. 
	 **/
	template<typename K = float>
	auto Scale(const K &scalar) const
	{
		return Vector2<decltype(m_x * scalar)>(m_x * scalar, m_y * scalar);
	}

	/**
	 * Rotates this vector by a angle around the origin.
	 * @tparam K The angle type.
	 * @param angle The angle to rotate by, in radians. 
	 * @return The rotated vector. 
	 **/
	template<typename K = float>
	auto Rotate(const K &angle) const
	{
		return Vector2<decltype(m_x * angle)>(m_x * std::cos(angle) - m_y * std::sin(angle), m_x * std::sin(angle) + m_y * std::cos(angle));
	}

	/**
	 * Rotates this vector by a angle around a rotation axis.
	 * @tparam K The angle type.
	 * @tparam J The rotation axis type.
	 * @param angle The angle to rotate by, in radians. 
	 * @param rotationAxis The point to rotate the vector around. 
	 * @return The rotated vector. 
	 **/
	template<typename K = float, typename J>
	auto Rotate(const K &angle, const Vector2<J> &rotationAxis) const
	{
		auto x = ((m_x - rotationAxis.m_x) * std::cos(angle)) - ((m_y - rotationAxis.m_y) * std::sin(angle) + rotationAxis.m_x);
		auto y = ((m_x - rotationAxis.m_x) * std::sin(angle)) + ((m_y - rotationAxis.m_y) * std::cos(angle) + rotationAxis.m_y);
		return Vector2<decltype(x)>(x, y);
	}

	/**
	 * Negates this vector.
	 * @return The negated vector. 
	 **/
	auto Negate() const
	{
		return Vector2<decltype(-m_x)>(-m_x, -m_y);
	}

	/**
	 * Normalizes this vector.
	 * @return The normalized vector. 
	 **/
	auto Normalize() const
	{
		auto l = Length();

		if (l == 0)
		{
			throw std::runtime_error("Can't normalize a zero length vector");
		}

		return *this / l;
	}

	/**
	 * Gets the length squared of this vector.
	 * @return The length squared. 
	 **/
	auto LengthSquared() const
	{
		return m_x * m_x + m_y * m_y;
	}

	/**
	 * Gets the length of this vector.
	 * @return The length. 
	 **/
	auto Length() const
	{
		return std::sqrt(LengthSquared());
	}

	/**
	 * Gets the minimal value in this vector.
	 * @return The minimal components.
	 **/
	auto Min() const
	{
		return std::min({ m_x, m_y });
	}

	/**
	 * Gets the maximal value in this vector.
	 * @return The maximal components. 
	 **/
	auto Max() const
	{
		return std::max({ m_x, m_y });
	}

	/**
	 * Gets the minimal and maximal values in the vector.
	 * @return The minimal and maximal components.
	 */
	auto MinMax() const
	{
		return std::minmax({ m_x, m_y });
	}

	/**
	 * Gets the lowest vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The lowest vector.
	 **/
	template<typename K>
	auto Min(const Vector2<K> &other)
	{
		return Vector2<decltype(std::min(m_x, other.m_x))>(std::min(m_x, other.m_x), std::min(m_y, other.m_y));
	}

	/**
	 * Gets the maximum vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The maximum vector.
	 **/
	template<typename K>
	auto Max(const Vector2<K> &other)
	{
		return Vector2<decltype(std::max(m_x, other.m_x))>(std::max(m_x, other.m_x), std::max(m_y, other.m_y));
	}

	/**
	 * Gets the distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The squared distance. 
	 **/
	template<typename K>
	auto DistanceSquared(const Vector2<K> &other) const
	{
		auto dx = m_x - other.m_x;
		auto dy = m_y - other.m_y;
		return dx * dx + dy * dy;
	}

	/**
	 * Gets the between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The distance. 
	 **/
	template<typename K>
	auto Distance(const Vector2<K> &other) const
	{
		return std::sqrt(DistanceSquared(other));
	}

	/**
	 * Gets the vector distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The vector distance. 
	 **/
	template<typename K>
	auto DistanceVector(const Vector2<K> &other) const
	{
		return (*this - other) * (*this - other);
	}

	/**
	 * Gets if this vector is in a triangle.
	 * @tparam K The types.
	 * @param v1 The first triangle vertex. 
	 * @param v2 The second triangle vertex. 
	 * @param v3 The third triangle vertex. 
	 * @return If this vector is in a triangle. 
	 **/
	template<typename K>
	bool InTriangle(const Vector2<K> &v1, const Vector2<K> &v2, const Vector2<K> &v3) const
	{
		bool b1 = ((m_x - v2.m_x) * (v1.m_y - v2.m_y) - (v1.m_x - v2.m_x) * (m_y - v2.m_y)) < 0;
		bool b2 = ((m_x - v3.m_x) * (v2.m_y - v3.m_y) - (v2.m_x - v3.m_x) * (m_y - v3.m_y)) < 0;
		bool b3 = ((m_x - v1.m_x) * (v3.m_y - v1.m_y) - (v3.m_x - v1.m_x) * (m_y - v1.m_y)) < 0;
		return ((b1 == b2) & (b2 == b3));
	}

	/**
	 * Gradually changes this vector to a target.
	 * @param target The target vector. 
	 * @param rate The rate to go from current to the target. 
	 * @return The changed vector. 
	 **/
	template<typename K, typename J>
	auto SmoothDamp(const Vector2<K> &target, const Vector2<J> &rate) const
	{
		return Maths::SmoothDamp(*this, target, rate);
	}

	/**
	 * Converts from rectangular to spherical coordinates, this vector is in cartesian (x, y).
	 * @return The polar coordinates (radius, theta). 
	 **/
	auto CartesianToPolar() const
	{
		auto radius = std::sqrt(m_x * m_x + m_y * m_y);
		auto theta = std::atan2(m_y, m_x);
		return Vector2<decltype(radius)>(radius, theta);
	}

	/**
	 * Converts from spherical to rectangular coordinates, this vector is in polar (radius, theta).
	 * @return The cartesian coordinates (x, y). 
	 **/
	auto PolarToCartesian() const
	{
		auto x = m_x * std::cos(m_y);
		auto y = m_x * std::sin(m_y);
		return Vector2<decltype(x)>(x, y);
	}

	const T &GetX() const { return m_x; }

	void SetX(const T &x) { m_x = x; }

	const T &GetY() const { return m_y; }

	void SetY(const T &y) { m_y = y; }

	void Decode(const Metadata &metadata)
	{
		metadata.GetChild("x", m_x);
		metadata.GetChild("y", m_y);
	}

	void Encode(Metadata &metadata) const
	{
		metadata.SetChild("x", m_x);
		metadata.SetChild("y", m_y);
	}

	std::string ToString() const
	{
		std::stringstream stream;
		stream.precision(10);
		stream << "Vector2(" << m_x << ", " << m_y << ")";
		return stream.str();
	}

	template<typename K>
	bool operator==(const Vector2<K> &other) const
	{
		return m_x == other.m_x && m_y == other.m_y;
	}

	template<typename K>
	bool operator!=(const Vector2<K> &other) const
	{
		return !(*this == other);
	}

	Vector2 operator-() const
	{
		return Negate();
	}

	const T &operator[](const uint32_t &index) const
	{
		switch (index)
		{
		case 0:
			return m_x;
		case 1:
			return m_y;
		default:
			throw std::runtime_error("Vector2 index out of bounds!");
		}
	}

	T &operator[](const uint32_t &index)
	{
		switch (index)
		{
		case 0:
			return m_x;
		case 1:
			return m_y;
		default:
			throw std::runtime_error("Vector2 index out of bounds!");
		}
	}

	template<typename K>
	Vector2 &operator+=(const Vector2<K> &other)
	{
		return *this = Add(other);
	}

	template<typename K>
	Vector2 &operator-=(const Vector2<K> &other)
	{
		return *this = Subtract(other);
	}

	template<typename K>
	Vector2 &operator*=(const Vector2<K> &other)
	{
		return *this = Multiply(other);
	}

	template<typename K>
	Vector2 &operator/=(const Vector2<K> &other)
	{
		return *this = Divide(other);
	}

	Vector2 &operator+=(const T &other)
	{
		return *this = Add(Vector2<T>(other));
	}

	Vector2 &operator-=(const T &other)
	{
		return *this = Subtract(Vector2<T>(other));
	}

	Vector2 &operator*=(const T &other)
	{
		return *this = Multiply(Vector2<T>(other));
	}

	Vector2 &operator/=(const T &other)
	{
		return *this = Divide(Vector2<T>(other));
	}

	ACID_EXPORT static const Vector2 Zero;
	ACID_EXPORT static const Vector2 One;
	ACID_EXPORT static const Vector2 Left;
	ACID_EXPORT static const Vector2 Right;
	ACID_EXPORT static const Vector2 Up;
	ACID_EXPORT static const Vector2 Down;
	ACID_EXPORT static const Vector2 PositiveInfinity;
	ACID_EXPORT static const Vector2 NegativeInfinity;

	T m_x, m_y;
};

template<typename K>
std::ostream &operator<<(std::ostream &stream, const Vector2<K> &vector)
{
	stream << vector.ToString();
	return stream;
}

template<typename K, typename J>
auto operator+(const Vector2<K> &left, const Vector2<J> &right)
{
	return left.Add(right);
}

template<typename K, typename J>
auto operator-(const Vector2<K> &left, const Vector2<J> &right)
{
	return left.Subtract(right);
}

template<typename K, typename J>
auto operator*(const Vector2<K> &left, const Vector2<J> &right)
{
	return left.Multiply(right);
}

template<typename K, typename J>
auto operator/(const Vector2<K> &left, const Vector2<J> &right)
{
	return left.Divide(right);
}

template<typename K, typename J>
auto operator+(const K &left, const Vector2<J> &right)
{
	return Vector2<K>(left).Add(right);
}

template<typename K, typename J>
auto operator-(const K &left, const Vector2<J> &right)
{
	return Vector2<K>(left).Subtract(right);
}

template<typename K, typename J>
auto operator*(const K &left, const Vector2<J> &right)
{
	return Vector2<K>(left).Multiply(right);
}

template<typename K, typename J>
auto operator/(const K &left, const Vector2<J> &right)
{
	return Vector2<K>(left).Divide(right);
}

template<typename K, typename J>
auto operator+(const Vector2<K> &left, const J &right)
{
	return left.Add(Vector2<J>(right));
}

template<typename K, typename J>
auto operator-(const Vector2<K> &left, const J &right)
{
	return left.Subtract(Vector2<J>(right));
}

template<typename K, typename J>
auto operator*(const Vector2<K> &left, const J &right)
{
	return left.Multiply(Vector2<J>(right));
}

template<typename K, typename J>
auto operator/(const Vector2<K> &left, const J &right)
{
	return left.Divide(Vector2<J>(right));
}

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2i = Vector2<int32_t>;
using Vector2ui = Vector2<uint32_t>;
}

namespace std
{
template<typename T>
struct hash<acid::Vector2<T>>
{
	size_t operator()(acid::Vector2<T> const &vector) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.m_x);
		acid::Maths::HashCombine(seed, vector.m_y);
		return seed;
	}
};
}
