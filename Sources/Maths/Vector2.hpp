#pragma once

#include "StdAfx.hpp"

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
	Vector2(); 
	
	/**
	 * Constructor for Vector2.
	 * @param a The value to set all components to.
	 **/
	explicit Vector2(const T &a); 
	
	/**
	 * Constructor for Vector2.
	 * @param x Start x. 
	 * @param y Start y. 
	 **/
	Vector2(const T &x, const T &y); 
	
	/**
	 * Constructor for Vector2.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	Vector2(const Vector2<K> &source); 
	
	/**
	 * Constructor for Vector3.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	explicit Vector2(const Vector3<K> &source); 
	
	/**
	 * Adds this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	template<typename K>
	auto Add(const Vector2<K> &other) const; 
	
	/**
	 * Subtracts this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	template<typename K>
	auto Subtract(const Vector2<K> &other) const; 
	
	/**
	 * Multiplies this vector with another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	template<typename K>
	auto Multiply(const Vector2<K> &other) const; 
	
	/**
	 * Divides this vector by another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	template<typename K>
	auto Divide(const Vector2<K> &other) const; 
	
	/**
	 * Calculates the angle between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The angle, in radians. 
	 **/
	template<typename K>
	auto Angle(const Vector2<K> &other) const; 
	
	/**
	 * Calculates the dot product of the this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The dot product. 
	 **/
	template<typename K>
	auto Dot(const Vector2<K> &other) const; 
	
	/**
	 * Calculates the linear interpolation between this vector and another vector.
	 * @tparam K The others type.
	 * @tparam J The progression type.
	 * @param other The other quaternion. 
	 * @param progression The progression. 
	 * @return Left lerp right. 
	 **/
	template<typename K, typename J = float>
	auto Lerp(const Vector2<K> &other, const J &progression) const; 
	
	/**
	 * Scales this vector by a scalar.
	 * @tparam K The scalar type.
	 * @param scalar The scalar value. 
	 * @return The scaled vector. 
	 **/
	template<typename K = float>
	auto Scale(const K &scalar) const; 
	
	/**
	 * Rotates this vector by a angle around the origin.
	 * @tparam K The angle type.
	 * @param angle The angle to rotate by, in radians. 
	 * @return The rotated vector. 
	 **/
	template<typename K = float>
	auto Rotate(const K &angle) const; 
	
	/**
	 * Rotates this vector by a angle around a rotation axis.
	 * @tparam K The angle type.
	 * @tparam J The rotation axis type.
	 * @param angle The angle to rotate by, in radians. 
	 * @param rotationAxis The point to rotate the vector around. 
	 * @return The rotated vector. 
	 **/
	template<typename K = float, typename J>
	auto Rotate(const K &angle, const Vector2<J> &rotationAxis) const; 
	
	/**
	 * Normalizes this vector.
	 * @return The normalized vector. 
	 **/
	auto Normalize() const; 
	
	/**
	 * Gets the length squared of this vector.
	 * @return The length squared. 
	 **/
	auto LengthSquared() const; 
	
	/**
	 * Gets the length of this vector.
	 * @return The length. 
	 **/
	auto Length() const;

	/**
	 * Gets the absolute value of every component in this vector.
	 * @return The absolute value of this vector.
	 **/
	auto Abs() const;

	/**
	 * Gets the minimal value in this vector.
	 * @return The minimal components.
	 **/
	auto Min() const; 
	
	/**
	 * Gets the maximal value in this vector.
	 * @return The maximal components. 
	 **/
	auto Max() const; 
	
	/**
	 * Gets the minimal and maximal values in the vector.
	 * @return The minimal and maximal components.
	 */
	auto MinMax() const; 
	
	/**
	 * Gets the lowest vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The lowest vector.
	 **/
	template<typename K>
	auto Min(const Vector2<K> &other); 
	
	/**
	 * Gets the maximum vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The maximum vector.
	 **/
	template<typename K>
	auto Max(const Vector2<K> &other); 
	
	/**
	 * Gets the distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The squared distance. 
	 **/
	template<typename K>
	auto DistanceSquared(const Vector2<K> &other) const; 
	
	/**
	 * Gets the between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The distance. 
	 **/
	template<typename K>
	auto Distance(const Vector2<K> &other) const; 
	
	/**
	 * Gets the vector distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector. 
	 * @return The vector distance. 
	 **/
	template<typename K>
	auto DistanceVector(const Vector2<K> &other) const; 
	
	/**
	 * Gets if this vector is in a triangle.
	 * @tparam K The types.
	 * @param v1 The first triangle vertex. 
	 * @param v2 The second triangle vertex. 
	 * @param v3 The third triangle vertex. 
	 * @return If this vector is in a triangle. 
	 **/
	template<typename K>
	bool InTriangle(const Vector2<K> &v1, const Vector2<K> &v2, const Vector2<K> &v3) const; 
	
	/**
	 * Gradually changes this vector to a target.
	 * @param target The target vector. 
	 * @param rate The rate to go from current to the target. 
	 * @return The changed vector. 
	 **/
	template<typename K, typename J>
	auto SmoothDamp(const Vector2<K> &target, const Vector2<J> &rate) const; 
	
	/**
	 * Converts from rectangular to spherical coordinates, this vector is in cartesian (x, y).
	 * @return The polar coordinates (radius, theta). 
	 **/
	auto CartesianToPolar() const; 
	
	/**
	 * Converts from spherical to rectangular coordinates, this vector is in polar (radius, theta).
	 * @return The cartesian coordinates (x, y). 
	 **/
	auto PolarToCartesian() const;

	const T &GetX() const { return m_x; }

	void SetX(const T &x) { m_x = x; }

	const T &GetY() const { return m_y; }

	void SetY(const T &y) { m_y = y; }

	std::string ToString() const;

	template<typename K>
	bool operator==(const Vector2<K> &other) const;

	template<typename K>
	bool operator!=(const Vector2<K> &other) const;

	template<typename U = T>
	std::enable_if_t<std::is_signed_v<U>, Vector2> operator-() const;

	template<typename U = T>
	std::enable_if_t<std::is_integral_v<U>, Vector2> operator~() const;

	const T &operator[](const uint32_t &index) const;

	T &operator[](const uint32_t &index);

	template<typename K>
	Vector2 &operator+=(const Vector2<K> &other);

	template<typename K>
	Vector2 &operator-=(const Vector2<K> &other);

	template<typename K>
	Vector2 &operator*=(const Vector2<K> &other);

	template<typename K>
	Vector2 &operator/=(const Vector2<K> &other);

	Vector2 &operator+=(const T &other);

	Vector2 &operator-=(const T &other);

	Vector2 &operator*=(const T &other);

	Vector2 &operator/=(const T &other);

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

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2i = Vector2<int32_t>;
using Vector2ui = Vector2<uint32_t>;
}

#include "Vector2.inl"
