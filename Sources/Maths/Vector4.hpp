#pragma once

#include "StdAfx.hpp"

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
	constexpr Vector4() = default;

	/**
	 * Constructor for Vector4.
	 * @param a The value to set all components to.
	 **/
	constexpr Vector4(const T &a);

	/**
	 * Constructor for Vector4.
	 * @param x Start x.
	 * @param y Start y.
	 * @param z Start z.
	 * @param w Start w.
	 **/
	constexpr Vector4(const T &x, const T &y, const T &z, const T &w = 1);

	/**
	 * Constructor for Vector4.
	 * @tparam K The x type.
	 * @tparam J The y type.
	 * @tparam H The z type.
	 * @tparam F The w type.
	 * @param x Start x.
	 * @param y Start y.
	 * @param z Start z.
	 * @param w Start w.
	 **/
	template<typename K, typename J, typename H, typename F>
	constexpr Vector4(const K &x, const J &y, const H &z, const F &w);

	/**
	 * Constructor for Vector4.
	 * @tparam K The left type.
	 * @tparam J The right type.
	 * @param left Creates this vector out of a existing vector, xy.
	 * @param right Creates this vector out of a existing vector, zw.
	 **/
	template<typename K, typename J>
	constexpr Vector4(const Vector2<K> &left, const Vector2<J> &right = Vector2<K>::Up);

	/**
	 * Constructor for Vector4.
	 * @tparam K The sources type.
	 * @tparam J The w type.
	 * @param source Creates this vector out of a existing vector.
	 * @param w Start w.
	 **/
	template<typename K, typename J = T>
	explicit constexpr Vector4(const Vector3<K> & source, const J & w = 1);

	/**
	 * Constructor for Vector4.
	 * @tparam K The sources type.
	 * @param source Creates this vector out of a existing vector.
	 **/
	template<typename K>
	constexpr Vector4(const Vector4<K> &source);

	/**
	 * Adds this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	constexpr auto Add(const Vector4<K> &other) const;

	/**
	 * Subtracts this vector to another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	constexpr auto Subtract(const Vector4<K> &other) const;

	/**
	 * Multiplies this vector with another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	constexpr auto Multiply(const Vector4<K> &other) const;

	/**
	 * Divides this vector by another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The resultant vector.
	 **/
	template<typename K>
	constexpr auto Divide(const Vector4<K> &other) const;

	/**
	 * Calculates the angle between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The angle, in radians.
	 **/
	template<typename K>
	auto Angle(const Vector4<K> &other) const;

	/**
	 * Calculates the dot product of the this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The dot product.
	 **/
	template<typename K>
	constexpr auto Dot(const Vector4<K> &other) const;

	/**
	 * Calculates the linear interpolation between this vector and another vector.
	 * @tparam K The others type.
	 * @tparam J The progression type.
	 * @param other The other quaternion.
	 * @param progression The progression.
	 * @return Left lerp right.
	 **/
	template<typename K, typename J = float>
	constexpr auto Lerp(const Vector4<K> & other, const J & progression) const;

	/**
	 * Scales this vector by a scalar.
	 * @tparam K The scalar type.
	 * @param scalar The scalar value.
	 * @return The scaled vector.
	 **/
	template<typename K = float>
	constexpr auto Scale(const K & scalar) const;

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
	constexpr auto Min(const Vector4<K> &other);

	/**
	 * Gets the maximum vector size between this vector and other.
	 * @tparam K The others type.
	 * @param other The other vector to get values from.
	 * @return The maximum vector.
	 **/
	template<typename K>
	constexpr auto Max(const Vector4<K> &other);

	/**
	 * Gets the distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The squared distance.
	 **/
	template<typename K>
	constexpr auto DistanceSquared(const Vector4<K> &other) const;

	/**
	 * Gets the between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The distance.
	 **/
	template<typename K>
	auto Distance(const Vector4<K> &other) const;

	/**
	 * Gets the vector distance between this vector and another vector.
	 * @tparam K The others type.
	 * @param other The other vector.
	 * @return The vector distance.
	 **/
	template<typename K>
	constexpr auto DistanceVector(const Vector4<K> &other) const;

	/**
	 * Gradually changes this vector to a target.
	 * @param target The target vector.
	 * @param rate The rate to go from current to the target.
	 * @return The changed vector.
	 **/
	template<typename K, typename J>
	constexpr auto SmoothDamp(const Vector4<K> &target, const Vector4<J> &rate) const;

	std::string ToString() const;

	template<typename K>
	constexpr bool operator==(const Vector4<K> &other) const;

	template<typename K>
	constexpr bool operator!=(const Vector4<K> &other) const;

	template<typename U = T>
	constexpr std::enable_if_t<std::is_signed_v<U>, Vector4> operator-() const;

	template<typename U = T>
	constexpr std::enable_if_t<std::is_integral_v<U>, Vector4> operator~() const;

	constexpr const T &operator[](const uint32_t &index) const;

	constexpr T &operator[](const uint32_t &index);

	template<typename K>
	constexpr Vector4 &operator+=(const Vector4<K> &other);

	template<typename K>
	constexpr Vector4 &operator-=(const Vector4<K> &other);

	template<typename K>
	constexpr Vector4 &operator*=(const Vector4<K> &other);

	template<typename K>
	constexpr Vector4 &operator/=(const Vector4<K> &other);

	constexpr Vector4 &operator+=(const T &other);

	constexpr Vector4 &operator-=(const T &other);

	constexpr Vector4 &operator*=(const T &other);

	constexpr Vector4 &operator/=(const T &other);

	ACID_EXPORT static const Vector4 Zero;
	ACID_EXPORT static const Vector4 One;
	ACID_EXPORT static const Vector4 PositiveInfinity;
	ACID_EXPORT static const Vector4 NegativeInfinity;

	T m_x{}, m_y{}, m_z{}, m_w{1};
};

using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;
using Vector4i = Vector4<int32_t>;
using Vector4ui = Vector4<uint32_t>;
}

#include "Vector4.inl"
