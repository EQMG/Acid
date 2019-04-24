#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"

namespace acid
{
template<typename T>
class Vector2;

template<typename T>
class Vector4;

/**
 * @brief Holds a 3-tuple vector.
 * @tparam T The value type.
 */
template<typename T>
class Vector3
{
public:
	/**
	 * Constructor for Vector3.
	 **/
	explicit Vector3() :
		m_x(0),
		m_y(0),
		m_z(0)
	{
	}

	/**
	 * Constructor for Vector3.
	 * @param a The value to set all components to.
	 **/
	explicit Vector3(const T &a) :
		m_x(a),
		m_y(a),
		m_z(a)
	{
	}

	/**
	 * Constructor for Vector3.
	 * @param x Start x.
	 * @param y Start y.
	 * @param z Start z.
	 **/
	Vector3(const T &x, const T &y, const T &z) :
		m_x(x),
		m_y(y),
		m_z(z)
	{
	}

	/**
	 * Constructor for Vector3.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 * @param z Start z.
	 **/
	template<typename K>
	explicit Vector3(const Vector2<K> &source, const float &z = 0) :
		m_x(static_cast<T>(source.m_x)),
		m_y(static_cast<T>(source.m_y)),
		m_z(z)
	{
	}

	/**
	 * Constructor for Vector3.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	Vector3(const Vector3<K> &source) :
		m_x(static_cast<T>(source.m_x)),
		m_y(static_cast<T>(source.m_y)),
		m_z(static_cast<T>(source.m_z))
	{
	}

	/**
	 * Constructor for Vector3.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	explicit Vector3(const Vector4<K> &source) :
		m_x(static_cast<T>(source.m_x)),
		m_y(static_cast<T>(source.m_y)),
		m_z(static_cast<T>(source.m_z))
	{
	}

	/**
	 * Adds this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	auto Add(const Vector3<K> &other) const
	{
		return Vector3<decltype(m_x + other.m_x)>(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
	}

	/**
	 * Subtracts this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	auto Subtract(const Vector3<K> &other) const
	{
		return Vector3<decltype(m_x - other.m_x)>(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
	}

	/**
	 * Multiplies this vector with another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	auto Multiply(const Vector3<K> &other) const
	{
		return Vector3<decltype(m_x * other.m_x)>(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z);
	}

	/**
	 * Divides this vector by another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	auto Divide(const Vector3<K> &other) const
	{
		return Vector3<decltype(m_x / other.m_x)>(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z);
	}

	/**
	 * Calculates the angle between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The angle, in radians.
	 **/
	template<typename K>
	auto Angle(const Vector3<K> &other) const
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
	auto Dot(const Vector3<K> &other) const
	{
		return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
	}

	/**
	 * Calculates the cross product of the this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The cross product.
	 **/
	template<typename K>
	auto Cross(const Vector3<K> &other) const
	{
		return Vector3<decltype(m_x * other.m_x)>(m_y * other.m_z - m_z * other.m_y, other.m_x * m_z - other.m_z * m_x, m_x * other.m_y - m_y * other.m_x);
	}

	/**
	 * Calculates the linear interpolation between this vector and another vector.
	 * @tparam K The others type.
	 * @tparam J The progression type.
	 * @param other The other vector.
	 * @param progression The progression.
	 * @return Left lerp right.
	 **/
	template<typename K, typename J = float>
	auto Lerp(const Vector3<K> &other, const J &progression) const
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
		return Vector3<decltype(m_x * scalar)>(m_x * scalar, m_y * scalar, m_z * scalar);
	}

	/**
	 * Rotates this vector by a angle around the origin.
	 * @tparam K The rotations type.
	 * @param angle The angle to rotate by, in radians.
	 * @return The rotated vector.
	 **/
	/*template<typename K = float>
	auto Rotate(const Vector3<K> &rotation) const
	{
		Matrix4 matrix = Matrix4::TransformationMatrix(Zero, rotation, One);
		return matrix.Transform(*this);
	}*/

	/**
	 * Negates this vector.
	 * @return The negated vector.
	 **/
	auto Negate() const
	{
		return Vector3<decltype(-m_x)>(-m_x, -m_y, -m_z);
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
		return m_x * m_x + m_y * m_y + m_z * m_z;
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
		return std::min({ m_x, m_y, m_z });
	}

	/**
	 * Gets the maximal value in this vector.
	 * @return The maximal components.
	 **/
	auto Max() const
	{
		return std::max({ m_x, m_y, m_z });
	}

	/**
	 * Gets the minimal and maximal values in the vector.
	 * @return The minimal and maximal components.
	 */
	auto MinMax() const
	{
		return std::minmax({ m_x, m_y, m_z });
	}

	/**
	 * Gets the lowest vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The lowest vector.
	 **/
	template<typename K>
	auto Min(const Vector3<K> &other)
	{
		return Vector3<decltype(std::min(m_x, other.m_x))>(std::min(m_x, other.m_x), std::min(m_y, other.m_y), std::min(m_z, other.m_z));
	}

	/**
	 * Gets the maximum vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The maximum vector.
	 **/
	template<typename K>
	auto Max(const Vector3<K> &other)
	{
		return Vector3<decltype(std::max(m_x, other.m_x))>(std::max(m_x, other.m_x), std::max(m_y, other.m_y), std::max(m_z, other.m_z));
	}

	/**
	 * Gets the distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The squared distance.
	 **/
	template<typename K>
	auto DistanceSquared(const Vector3<K> &other) const
	{
		auto dx = m_x - other.m_x;
		auto dy = m_y - other.m_y;
		auto dz = m_z - other.m_z;
		return dx * dx + dy * dy + dz * dz;
	}

	/**
	 * Gets the between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The distance.
	 **/
	template<typename K>
	auto Distance(const Vector3<K> &other) const
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
	auto DistanceVector(const Vector3<K> &other) const
	{
		return (*this - other) * (*this - other);
	}

	/**
	 * Gradually changes this vector to a target.
	 * @param target The target vector.
	 * @param rate The rate to go from current to the target.
	 * @return The changed vector.
	 **/
	template<typename K, typename J>
	auto SmoothDamp(const Vector3<K> &target, const Vector3<J> &rate) const
	{
		return Maths::SmoothDamp(*this, target, rate);
	}

	/**
	 * Converts from rectangular to spherical coordinates, this vector is in cartesian (x, y).
	 * @return The polar coordinates (radius, theta).
	 **/
	auto CartesianToPolar() const
	{
		auto radius = std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
		auto theta = std::atan2(m_y, m_x);
		auto phi = std::atan2(std::sqrt(m_x * m_x + m_y * m_y), m_z);
		return Vector3<decltype(radius)>(radius, theta, phi);
	}

	/**
	 * Converts from spherical to rectangular coordinates, this vector is in polar (radius, theta).
	 * @return The cartesian coordinates (x, y).
	 **/
	auto PolarToCartesian() const
	{
		auto x = m_x * std::sin(m_z) * std::cos(m_y);
		auto y = m_x * std::sin(m_z) * std::sin(m_y);
		auto z = m_x * std::cos(m_z);
		return Vector3<decltype(x)>(x, y, z);
	}

	const T &GetX() const { return m_x; }

	void SetX(const T &x) { m_x = x; }

	const T &GetY() const { return m_y; }

	void SetY(const T &y) { m_y = y; }

	const T &GetZ() const { return m_z; }

	void SetZ(const T &z) { m_z = z; }

	void Decode(const Metadata &metadata)
	{
		metadata.GetChild("x", m_x);
		metadata.GetChild("y", m_y);
		metadata.GetChild("z", m_z);
	}

	void Encode(Metadata &metadata) const
	{
		metadata.SetChild("x", m_x);
		metadata.SetChild("y", m_y);
		metadata.SetChild("z", m_z);
	}

	std::string ToString() const
	{
		std::stringstream stream;
		stream.precision(10);
		stream << "Vector3(" << m_x << ", " << m_y << ", " << m_z << ")";
		return stream.str();
	}

	template<typename K>
	bool operator==(const Vector3<K> &other) const
	{
		return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z;
	}

	template<typename K>
	bool operator!=(const Vector3<K> &other) const
	{
		return !(*this == other);
	}

	Vector3 operator-() const
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
		case 2:
			return m_z;
		default:
			throw std::runtime_error("Vector3 index out of bounds!");
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
		case 2:
			return m_z;
		default:
			throw std::runtime_error("Vector3 index out of bounds!");
		}
	}

	template<typename K>
	Vector3 &operator+=(const Vector3<K> &other)
	{
		return *this = Add(other);
	}

	template<typename K>
	Vector3 &operator-=(const Vector3<K> &other)
	{
		return *this = Subtract(other);
	}

	template<typename K>
	Vector3 &operator*=(const Vector3<K> &other)
	{
		return *this = Multiply(other);
	}

	template<typename K>
	Vector3 &operator/=(const Vector3<K> &other)
	{
		return *this = Divide(other);
	}

	Vector3 &operator+=(const T &other)
	{
		return *this = Add(Vector3<T>(other));
	}

	Vector3 &operator-=(const T &other)
	{
		return *this = Subtract(Vector3<T>(other));
	}

	Vector3 &operator*=(const T &other)
	{
		return *this = Multiply(Vector3<T>(other));
	}

	Vector3 &operator/=(const T &other)
	{
		return *this = Divide(Vector3<T>(other));
	}

	ACID_EXPORT static const Vector3 Zero;
	ACID_EXPORT static const Vector3 One;
	ACID_EXPORT static const Vector3 Left;
	ACID_EXPORT static const Vector3 Right;
	ACID_EXPORT static const Vector3 Up;
	ACID_EXPORT static const Vector3 Down;
	ACID_EXPORT static const Vector3 Front;
	ACID_EXPORT static const Vector3 Back;
	ACID_EXPORT static const Vector3 PositiveInfinity;
	ACID_EXPORT static const Vector3 NegativeInfinity;

	T m_x, m_y, m_z;
};

template<typename K>
std::ostream &operator<<(std::ostream &stream, const Vector3<K> &vector)
{
	stream << vector.ToString();
	return stream;
}

template<typename K, typename J>
auto operator+(const Vector3<K> &left, const Vector3<J> &right)
{
	return left.Add(right);
}

template<typename K, typename J>
auto operator-(const Vector3<K> &left, const Vector3<J> &right)
{
	return left.Subtract(right);
}

template<typename K, typename J>
auto operator*(const Vector3<K> &left, const Vector3<J> &right)
{
	return left.Multiply(right);
}

template<typename K, typename J>
auto operator/(const Vector3<K> &left, const Vector3<J> &right)
{
	return left.Divide(right);
}

template<typename K, typename J>
auto operator+(const K &left, const Vector3<J> &right)
{
	return Vector3<K>(left).Add(right);
}

template<typename K, typename J>
auto operator-(const K &left, const Vector3<J> &right)
{
	return Vector3<K>(left).Subtract(right);
}

template<typename K, typename J>
auto operator*(const K &left, const Vector3<J> &right)
{
	return Vector3<K>(left).Multiply(right);
}

template<typename K, typename J>
auto operator/(const K &left, const Vector3<J> &right)
{
	return Vector3<K>(left).Divide(right);
}

template<typename K, typename J>
auto operator+(const Vector3<K> &left, const J &right)
{
	return left.Add(Vector3<J>(right));
}

template<typename K, typename J>
auto operator-(const Vector3<K> &left, const J &right)
{
	return left.Subtract(Vector3<J>(right));
}

template<typename K, typename J>
auto operator*(const Vector3<K> &left, const J &right)
{
	return left.Multiply(Vector3<J>(right));
}

template<typename K, typename J>
auto operator/(const Vector3<K> &left, const J &right)
{
	return left.Divide(Vector3<J>(right));
}

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3i = Vector3<int32_t>;
using Vector3ui = Vector3<uint32_t>;
}

namespace std
{
template<typename T>
struct hash<acid::Vector3<T>>
{
	size_t operator()(acid::Vector3<T> const &vector) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.m_x);
		acid::Maths::HashCombine(seed, vector.m_y);
		acid::Maths::HashCombine(seed, vector.m_z);
		return seed;
	}
};
}
