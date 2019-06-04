#pragma once

#include "StdAfx.hpp"

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
	explicit constexpr Vector3();
	
	/**
	 * Constructor for Vector3.
	 * @param a The value to set all components to.
	 **/
	explicit constexpr Vector3(const T &a);
	
	/**
	 * Constructor for Vector3.
	 * @param x Start x.
	 * @param y Start y.
	 * @param z Start z.
	 **/
	constexpr Vector3(const T &x, const T &y, const T &z);
	
	/**
	 * Constructor for Vector3.
	 * @tparam K The sources type.
	 * @tparam J The z type.
	 * @param source Creates this vector out of a existing vector.
	 * @param z Start z.
	 **/
	template<typename K, typename J = T>
	explicit constexpr Vector3(const Vector2<K> &source, const J &z = 0);
	
	/**
	 * Constructor for Vector3.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	constexpr Vector3(const Vector3<K> &source);
	
	/**
	 * Constructor for Vector3.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	explicit constexpr Vector3(const Vector4<K> &source);
	
	/**
	 * Adds this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	constexpr auto Add(const Vector3<K> &other) const;
	
	/**
	 * Subtracts this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	constexpr auto Subtract(const Vector3<K> &other) const;
	
	/**
	 * Multiplies this vector with another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	constexpr auto Multiply(const Vector3<K> &other) const;
	
	/**
	 * Divides this vector by another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	constexpr auto Divide(const Vector3<K> &other) const;
	
	/**
	 * Calculates the angle between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The angle, in radians.
	 **/
	template<typename K>
	auto Angle(const Vector3<K> &other) const;
	
	/**
	 * Calculates the dot product of the this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The dot product.
	 **/
	template<typename K>
	constexpr auto Dot(const Vector3<K> &other) const;
	
	/**
	 * Calculates the cross product of the this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The cross product.
	 **/
	template<typename K>
	constexpr auto Cross(const Vector3<K> &other) const;
	
	/**
	 * Calculates the linear interpolation between this vector and another vector.
	 * @tparam K The others type.
	 * @tparam J The progression type.
	 * @param other The other vector.
	 * @param progression The progression.
	 * @return Left lerp right.
	 **/
	template<typename K, typename J = float>
	constexpr auto Lerp(const Vector3<K> &other, const J &progression) const;
	
	/**
	 * Scales this vector by a scalar.
	 * @tparam K The scalar type.
	 * @param scalar The scalar value.
	 * @return The scaled vector.
	 **/
	template<typename K = float>
	constexpr auto Scale(const K &scalar) const;
	
	/**
	 * Rotates this vector by a angle around the origin.
	 * @tparam K The rotations type.
	 * @param angle The angle to rotate by, in radians.
	 * @return The rotated vector.
	 **/
	//template<typename K = float>
	//auto Rotate(const Vector3<K> &angle) const; 
	
	/**
	 * Normalizes this vector.
	 * @return The normalized vector.
	 **/
	auto Normalize() const;
	
	/**
	 * Gets the length squared of this vector.
	 * @return The length squared.
	 **/
	constexpr auto LengthSquared() const;
	
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
	constexpr auto Min() const;
	
	/**
	 * Gets the maximal value in this vector.
	 * @return The maximal components.
	 **/
	constexpr auto Max() const;
	
	/**
	 * Gets the minimal and maximal values in the vector.
	 * @return The minimal and maximal components.
	 */
	constexpr auto MinMax() const;
	
	/**
	 * Gets the lowest vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The lowest vector.
	 **/
	template<typename K>
	constexpr auto Min(const Vector3<K> &other);
	
	/**
	 * Gets the maximum vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The maximum vector.
	 **/
	template<typename K>
	constexpr auto Max(const Vector3<K> &other);
	
	/**
	 * Gets the distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The squared distance.
	 **/
	template<typename K>
	constexpr auto DistanceSquared(const Vector3<K> &other) const;
	
	/**
	 * Gets the between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The distance.
	 **/
	template<typename K>
	auto Distance(const Vector3<K> &other) const;
	
	/**
	 * Gets the vector distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The vector distance.
	 **/
	template<typename K>
	constexpr auto DistanceVector(const Vector3<K> &other) const;
	
	/**
	 * Gradually changes this vector to a target.
	 * @param target The target vector.
	 * @param rate The rate to go from current to the target.
	 * @return The changed vector.
	 **/
	template<typename K, typename J>
	constexpr auto SmoothDamp(const Vector3<K> &target, const Vector3<J> &rate) const;
	
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

	std::string ToString() const;

	template<typename K>
	constexpr bool operator==(const Vector3<K> &other) const;

	template<typename K>
	constexpr bool operator!=(const Vector3<K> &other) const;

	template<typename U = T>
	constexpr std::enable_if_t<std::is_signed_v<U>, Vector3> operator-() const;

	template<typename U = T>
	constexpr std::enable_if_t<std::is_integral_v<U>, Vector3> operator~() const;

	constexpr const T &operator[](const uint32_t &index) const;

	constexpr T &operator[](const uint32_t &index);

	template<typename K>
	constexpr Vector3 &operator+=(const Vector3<K> &other);

	template<typename K>
	constexpr Vector3 &operator-=(const Vector3<K> &other);

	template<typename K>
	constexpr Vector3 &operator*=(const Vector3<K> &other);

	template<typename K>
	constexpr Vector3 &operator/=(const Vector3<K> &other);

	constexpr Vector3 &operator+=(const T &other);

	constexpr Vector3 &operator-=(const T &other);

	constexpr Vector3 &operator*=(const T &other);

	constexpr Vector3 &operator/=(const T &other);

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

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3i = Vector3<int32_t>;
using Vector3ui = Vector3<uint32_t>;
}

#include "Vector3.inl"
