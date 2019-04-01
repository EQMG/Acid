#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"

namespace acid
{
/**
 * @brief Holds a 2-tuple vector.
 **/
class ACID_EXPORT Vector2
{
public:
	/**
	 * Constructor for Vector2.
	 * @param x Start x. 
	 * @param y Start y. 
	 **/
	explicit Vector2(const float &x = 0.0f, const float &y = 0.0f);

	/**
	 * Adds this vector to another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector2 Add(const Vector2 &other) const;

	/**
	 * Subtracts this vector to another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector2 Subtract(const Vector2 &other) const;

	/**
	 * Multiplies this vector with another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector2 Multiply(const Vector2 &other) const;

	/**
	 * Divides this vector by another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector2 Divide(const Vector2 &other) const;

	/**
	 * Calculates the angle between this vector and another vector.
	 * @param other The other vector. 
	 * @return The angle, in radians. 
	 **/
	float Angle(const Vector2 &other) const;

	/**
	 * Calculates the dot product of the this vector and another vector.
	 * @param other The other vector. 
	 * @return The dot product. 
	 **/
	float Dot(const Vector2 &other) const;

	/**
	 * Calculates the linear interpolation between this vector and another vector.
	 * @param other The other quaternion. 
	 * @param progression The progression. 
	 * @return Left lerp right. 
	 **/
	Vector2 Lerp(const Vector2 &other, const float &progression) const;

	/**
	 * Scales this vector by a scalar.
	 * @param scalar The scalar value. 
	 * @return The scaled vector. 
	 **/
	Vector2 Scale(const float &scalar) const;

	/**
	 * Rotates this vector by a angle around the origin.
	 * @param angle The angle to rotate by, in radians. 
	 * @return The rotated vector. 
	 **/
	Vector2 Rotate(const float &angle) const;

	/**
	 * Rotates this vector by a angle around a rotation axis.
	 * @param angle The angle to rotate by, in radians. 
	 * @param rotationAxis The point to rotate the vector around. 
	 * @return The rotated vector. 
	 **/
	Vector2 Rotate(const float &angle, const Vector2 &rotationAxis) const;

	/**
	 * Negates this vector.
	 * @return The negated vector. 
	 **/
	Vector2 Negate() const;

	/**
	 * Normalizes this vector.
	 * @return The normalized vector. 
	 **/
	Vector2 Normalize() const;

	/**
	 * Gets the length squared of this vector.
	 * @return The length squared. 
	 **/
	float LengthSquared() const;

	/**
	 * Gets the length of this vector.
	 * @return The length. 
	 **/
	float Length() const;

	/**
	 * Gets the maximum value in this vector.
	 * @return The largest components. 
	 **/
	float MaxComponent() const;

	/**
	 * Gets the lowest value in this vector.
	 * @return The smallest components. 
	 **/
	float MinComponent() const;

	/**
	 * Gets the distance between this vector and another vector.
	 * @param other The other vector. 
	 * @return The squared distance. 
	 **/
	float DistanceSquared(const Vector2 &other) const;

	/**
	 * Gets the between this vector and another vector.
	 * @param other The other vector. 
	 * @return The distance. 
	 **/
	float Distance(const Vector2 &other) const;

	/**
	 * Gets the vector distance between this vector and another vector.
	 * @param other The other vector. 
	 * @return The vector distance. 
	 **/
	Vector2 DistanceVector(const Vector2 &other) const;

	/**
	 * Gets if this vector is in a triangle.
	 * @param v1 The first triangle vertex. 
	 * @param v2 The second triangle vertex. 
	 * @param v3 The third triangle vertex. 
	 * @return If this vector is in a triangle. 
	 **/
	bool InTriangle(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3) const;

	/**
	 * Gradually changes this vector to a target.
	 * @param target The target vector. 
	 * @param rate The rate to go from current to the target. 
	 * @return The changed vector. 
	 **/
	Vector2 SmoothDamp(const Vector2 &target, const Vector2 &rate) const;

	/**
	 * Converts from rectangular to spherical coordinates, this vector is in cartesian (x, y).
	 * @return The polar coordinates (radius, theta). 
	 **/
	Vector2 CartesianToPolar() const;

	/**
	 * Converts from spherical to rectangular coordinates, this vector is in polar (radius, theta).
	 * @return The cartesian coordinates (x, y). 
	 **/
	Vector2 PolarToCartesian() const;

	/**
	 * Gets the lowest vector size.
	 * @param a The first vector to get values from. 
	 * @param b The second vector to get values from. 
	 * @return The lowest vector. 
	 **/
	static Vector2 MinVector(const Vector2 &a, const Vector2 &b);

	/**
	 * Gets the maximum vector size.
	 * @param a The first vector to get values from. 
	 * @param b The second vector to get values from. 
	 * @return The maximum vector. 
	 **/
	static Vector2 MaxVector(const Vector2 &a, const Vector2 &b);

	float GetX() const { return m_x; }

	void SetX(const float &x) { m_x = x; }

	float GetY() const { return m_y; }

	void SetY(const float &y) { m_y = y; }

	void Decode(const Metadata &metadata);

	void Encode(Metadata &metadata) const;

	bool operator==(const Vector2 &other) const;

	bool operator!=(const Vector2 &other) const;

	Vector2 operator-() const;

	const float &operator[](const uint32_t &index) const;

	float &operator[](const uint32_t &index);

	ACID_EXPORT friend Vector2 operator+(const Vector2 &left, const Vector2 &right);

	ACID_EXPORT friend Vector2 operator-(const Vector2 &left, const Vector2 &right);

	ACID_EXPORT friend Vector2 operator*(const Vector2 &left, const Vector2 &right);

	ACID_EXPORT friend Vector2 operator/(const Vector2 &left, const Vector2 &right);

	ACID_EXPORT friend Vector2 operator+(const float &left, const Vector2 &right);

	ACID_EXPORT friend Vector2 operator-(const float &left, const Vector2 &right);

	ACID_EXPORT friend Vector2 operator*(const float &left, const Vector2 &right);

	ACID_EXPORT friend Vector2 operator/(const float &left, const Vector2 &right);

	ACID_EXPORT friend Vector2 operator+(const Vector2 &left, const float &right);

	ACID_EXPORT friend Vector2 operator-(const Vector2 &left, const float &right);

	ACID_EXPORT friend Vector2 operator*(const Vector2 &left, const float &right);

	ACID_EXPORT friend Vector2 operator/(const Vector2 &left, const float &right);

	Vector2 &operator+=(const Vector2 &other);

	Vector2 &operator-=(const Vector2 &other);

	Vector2 &operator*=(const Vector2 &other);

	Vector2 &operator/=(const Vector2 &other);

	Vector2 &operator+=(const float &other);

	Vector2 &operator-=(const float &other);

	Vector2 &operator*=(const float &other);

	Vector2 &operator/=(const float &other);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Vector2 &vector);

	std::string ToString() const;

	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 Left;
	static const Vector2 Right;
	static const Vector2 Up;
	static const Vector2 Down;
	static const Vector2 PositiveInfinity;
	static const Vector2 NegativeInfinity;

	union
	{
		struct
		{
			float m_elements[2];
		};

		struct
		{
			float m_x, m_y;
		};
	};
};
}

namespace std
{
template<>
struct hash<acid::Vector2>
{
	size_t operator()(acid::Vector2 const &vector) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.m_x);
		acid::Maths::HashCombine(seed, vector.m_y);
		return seed;
	}
};
}
