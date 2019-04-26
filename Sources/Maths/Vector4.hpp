#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"

namespace acid
{
template<typename T>
class Vector2;

template<typename T>
class Vector3;

/**
 * @brief Holds a 4-tuple vector.
 * @tparam T The value type.
 */
template<typename T>
class Vector4
{
public:
	/**
	 * Constructor for Vector4.
	 **/
	explicit Vector4() :
		m_x(0),
		m_y(0),
		m_z(0),
		m_w(1)
	{
	}

	/**
	 * Constructor for Vector4.
	 * @param a The value to set all components to.
	 **/
	explicit Vector4(const T &a) :
		m_x(a),
		m_y(a),
		m_z(a),
		m_w(a)
	{
	}

	/**
	 * Constructor for Vector4.
	 * @param x Start x.
	 * @param y Start y.
	 * @param z Start z.
	 * @param w Start w.
	 **/
	Vector4(const T &x, const T &y, const T &z, const T &w = 1.0f) :
		m_x(x),
		m_y(y),
		m_z(z),
		m_w(w)
	{
	}

	/**
	 * Constructor for Vector4.
	 * @tparam K The sources type.
	 * @param left Creates this vector out of a existing vector, xy.
	 * @param right Creates this vector out of a existing vector, zw.
	 **/
	template<typename K>
	explicit Vector4(const Vector2<K> &left, const Vector2<K> &right = Vector2<K>::Up) :
		m_x(static_cast<T>(left.m_x)),
		m_y(static_cast<T>(left.m_y)),
		m_z(static_cast<T>(right.m_x)),
		m_w(static_cast<T>(right.m_y))
	{
	}

	/**
	 * Constructor for Vector4.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 * @param w Start w.
	 **/
	template<typename K>
	explicit Vector4(const Vector3<K> &source, const T &w = 1) :
		m_x(static_cast<T>(source.m_x)),
		m_y(static_cast<T>(source.m_y)),
		m_z(static_cast<T>(source.m_z)),
		m_w(w)
	{
	}

	/**
	 * Constructor for Vector4.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	Vector4(const Vector4<K> &source) :
		m_x(static_cast<T>(source.m_x)),
		m_y(static_cast<T>(source.m_y)),
		m_z(static_cast<T>(source.m_z)),
		m_w(static_cast<T>(source.m_w))
	{
	}

	/**
	 * Adds this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	auto Add(const Vector4<K> &other) const
	{
		return Vector4<decltype(m_x + other.m_x)>(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
	}

	/**
	 * Subtracts this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	auto Subtract(const Vector4<K> &other) const
	{
		return Vector4<decltype(m_x - other.m_x)>(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
	}

	/**
	 * Multiplies this vector with another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	auto Multiply(const Vector4<K> &other) const
	{
		return Vector4<decltype(m_x * other.m_x)>(m_x * other.m_x, m_y * other.m_y, m_z * other.m_z, m_w * other.m_w);
	}

	/**
	 * Divides this vector by another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	auto Divide(const Vector4<K> &other) const
	{
		return Vector4<decltype(m_x / other.m_x)>(m_x / other.m_x, m_y / other.m_y, m_z / other.m_z, m_w / other.m_w);
	}

	/**
	 * Calculates the angle between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The angle, in radians.
	 **/
	template<typename K>
	auto Angle(const Vector4<K> &other) const
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
	auto Dot(const Vector4<K> &other) const
	{
		return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
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
	auto Lerp(const Vector4<K> &other, const J &progression) const
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
		return Vector4<decltype(m_x * scalar)>(m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar);
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
		return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
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
		return std::min({ m_x, m_y, m_z, m_w });
	}

	/**
	 * Gets the maximal value in this vector.
	 * @return The maximal components.
	 **/
	auto Max() const
	{
		return std::max({ m_x, m_y, m_z, m_w });
	}

	/**
	 * Gets the minimal and maximal values in the vector.
	 * @return The minimal and maximal components.
	 */
	auto MinMax() const
	{
		return std::minmax({ m_x, m_y, m_z, m_w });
	}

	/**
	 * Gets the lowest vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The lowest vector.
	 **/
	template<typename K>
	auto Min(const Vector4<K> &other)
	{
		return Vector4<decltype(std::min(m_x, other.m_x))>(std::min(m_x, other.m_x), std::min(m_y, other.m_y), std::min(m_z, other.m_z), std::min(m_w, other.m_w));
	}

	/**
	 * Gets the maximum vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The maximum vector.
	 **/
	template<typename K>
	auto Max(const Vector4<K> &other)
	{
		return Vector4<decltype(std::max(m_x, other.m_x))>(std::max(m_x, other.m_x), std::max(m_y, other.m_y), std::max(m_z, other.m_z), std::max(m_w, other.m_w));
	}

	 /**
	  * Gets the distance between this vector and another vector.
	  * @tparam K The others type.
	  * @param other The other vector.
	  * @return The squared distance.
	  **/
	template<typename K>
	auto DistanceSquared(const Vector4<K> &other) const
	{
		auto dx = m_x - other.m_x;
		auto dy = m_y - other.m_y;
		auto dz = m_z - other.m_z;
		auto dw = m_w - other.m_w;
		return dx * dx + dy * dy + dz * dz + dw * dw;
	}

	/**
	 * Gets the between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The distance.
	 **/
	template<typename K>
	auto Distance(const Vector4<K> &other) const
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
	auto DistanceVector(const Vector4<K> &other) const
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
	auto SmoothDamp(const Vector4<K> &target, const Vector4<J> &rate) const
	{
		return Maths::SmoothDamp(*this, target, rate);
	}

	const T &GetX() const { return m_x; }

	void SetX(const T &x) { m_x = x; }

	const T &GetY() const { return m_y; }

	void SetY(const T &y) { m_y = y; }
	
	const T &GetZ() const { return m_z; }

	void SetZ(const T &z) { m_z = z; }
	
	const T &GetW() const { return m_w; }
	
	void SetW(const T &w) { m_w = w; }

	std::string ToString() const
	{
		std::stringstream stream;
		stream.precision(10);
		stream << "Vector4(" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << ")";
		return stream.str();
	}

	template<typename K>
	bool operator==(const Vector4<K> &other) const
	{
		return m_x == other.m_x && m_y == other.m_y && m_z == other.m_z && m_w == other.m_w;
	}

	template<typename K>
	bool operator!=(const Vector4<K> &other) const
	{
		return !(*this == other);
	}

	template<typename U = T>
	std::enable_if_t<std::is_signed_v<U>, Vector4> operator-() const
	{
		return Vector4(-m_x, -m_y, -m_z, -m_w);
	}

	template<typename U = T>
	std::enable_if_t<std::is_integral_v<U>, Vector4> operator~() const
	{
		return Vector4(~m_x, ~m_y, ~m_z, ~m_w);
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
		case 3:
			return m_w;
		default:
			throw std::runtime_error("Vector4 index out of bounds!");
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
		case 3:
			return m_w;
		default:
			throw std::runtime_error("Vector4 index out of bounds!");
		}
	}

	template<typename K>
	Vector4 &operator+=(const Vector4<K> &other)
	{
		return *this = Add(other);
	}

	template<typename K>
	Vector4 &operator-=(const Vector4<K> &other)
	{
		return *this = Subtract(other);
	}

	template<typename K>
	Vector4 &operator*=(const Vector4<K> &other)
	{
		return *this = Multiply(other);
	}

	template<typename K>
	Vector4 &operator/=(const Vector4<K> &other)
	{
		return *this = Divide(other);
	}

	Vector4 &operator+=(const T &other)
	{
		return *this = Add(Vector4<T>(other));
	}

	Vector4 &operator-=(const T &other)
	{
		return *this = Subtract(Vector4<T>(other));
	}

	Vector4 &operator*=(const T &other)
	{
		return *this = Multiply(Vector4<T>(other));
	}

	Vector4 &operator/=(const T &other)
	{
		return *this = Divide(Vector4<T>(other));
	}

	ACID_EXPORT static const Vector4 Zero;
	ACID_EXPORT static const Vector4 One;
	ACID_EXPORT static const Vector4 PositiveInfinity;
	ACID_EXPORT static const Vector4 NegativeInfinity;

	T m_x, m_y, m_z, m_w;
};

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

using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;
using Vector4i = Vector4<int32_t>;
using Vector4ui = Vector4<uint32_t>;
}

namespace std
{
template<typename T>
struct hash<acid::Vector4<T>>
{
	size_t operator()(acid::Vector4<T> const &vector) const
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
