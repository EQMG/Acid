#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"

namespace acid
{
class Colour;
class Vector2;
class Vector3;

/**
 * @brief Holds a 4-tuple vector.
 **/
class ACID_EXPORT Vector4
{
public:
	/**
	 * Constructor for Vector4.
	 * @param x Start x. 
	 * @param y Start y. 
	 * @param z Start z. 
	 * @param w Start w. 
	 **/
	explicit Vector4(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f, const float &w = 1.0f);

	/**
	 * Constructor for Vector4.
	 * @param a The XY value. 
	 * @param b The ZW value. 
	 **/
	Vector4(const Vector2 &a, const Vector2 &b);

	/**
	 * Constructor for Vector4.
	 * @param source Creates this vector out of a existing one. >
	 * @param w Start w. 
	 **/
	Vector4(const Vector3 &source, const float &w = 1.0f);

	/**
	 * Constructor for Vector4.
	 * @param source Creates this vector out of a existing colour. 
	 **/
	Vector4(const Colour &source);

	/**
	 * Adds this vector to another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector4 Add(const Vector4 &other) const;

	/**
	 * Subtracts this vector to another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector4 Subtract(const Vector4 &other) const;

	/**
	 * Multiplies this vector with another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector4 Multiply(const Vector4 &other) const;

	/**
	 * Divides this vector by another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector4 Divide(const Vector4 &other) const;

	/**
	 * Calculates the angle between this vector and another vector.
	 * @param other The other vector. 
	 * @return The angle, in radians. 
	 **/
	float Angle(const Vector4 &other) const;

	/**
	 * Calculates the dot product of the this vector and another vector.
	 * @param other The other vector. 
	 * @return The dot product. 
	 **/
	float Dot(const Vector4 &other) const;

	/**
	 * Calculates the linear interpolation between this vector and another vector.
	 * @param other The other quaternion. 
	 * @param progression The progression. 
	 * @return Left lerp right. 
	 **/
	Vector4 Lerp(const Vector4 &other, const float &progression) const;

	/**
	 * Scales this vector by a scalar.
	 * @param scalar The scalar value. 
	 * @return The scaled vector. 
	 **/
	Vector4 Scale(const float &scalar) const;

	/**
	 * Negates this vector.
	 * @return The negated vector. 
	 **/
	Vector4 Negate() const;

	/**
	 * Normalizes this vector.
	 * @return The normalized vector. 
	 **/
	Vector4 Normalize() const;

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
	float DistanceSquared(const Vector4 &other) const;

	/**
	 * Gets the between this vector and another vector.
	 * @param other The other vector. 
	 * @return The distance. 
	 **/
	float Distance(const Vector4 &other) const;

	/**
	 * Gets the vector distance between this vector and another vector.
	 * @param other The other vector. 
	 * @return The vector distance. 
	 **/
	Vector4 DistanceVector(const Vector4 &other) const;

	/**
	 * Gradually changes this vector to a target.
	 * @param target The target vector. 
	 * @param rate The rate to go from current to the target. 
	 * @return The changed vector. 
	 **/
	Vector4 SmoothDamp(const Vector4 &target, const Vector4 &rate) const;

	/**
	 * Gets the lowest vector size.
	 * @param a The first vector to get values from. 
	 * @param b The second vector to get values from. 
	 * @return The lowest vector. 
	 **/
	static Vector4 MinVector(const Vector4 &a, const Vector4 &b);

	/**
	 * Gets the maximum vector size.
	 * @param a The first vector to get values from. 
	 * @param b The second vector to get values from. 
	 * @return The maximum vector. 
	 **/
	static Vector4 MaxVector(const Vector4 &a, const Vector4 &b);

	float GetX() const { return m_x; }

	void SetX(const float &x) { m_x = x; }

	float GetY() const { return m_y; }

	void SetY(const float &y) { m_y = y; }

	float GetZ() const { return m_z; }

	void SetZ(const float &z) { m_z = z; }

	float GetW() const { return m_w; }

	void SetW(const float &w) { m_w = w; }

	void Decode(const Metadata &metadata);

	void Encode(Metadata &metadata) const;

	bool operator==(const Vector4 &other) const;

	bool operator!=(const Vector4 &other) const;

	Vector4 operator-() const;

	const float &operator[](const uint32_t &index) const;

	float &operator[](const uint32_t &index);

	ACID_EXPORT friend Vector4 operator+(const Vector4 &left, const Vector4 &right);

	ACID_EXPORT friend Vector4 operator-(const Vector4 &left, const Vector4 &right);

	ACID_EXPORT friend Vector4 operator*(const Vector4 &left, const Vector4 &right);

	ACID_EXPORT friend Vector4 operator/(const Vector4 &left, const Vector4 &right);

	ACID_EXPORT friend Vector4 operator+(const float &left, const Vector4 &right);

	ACID_EXPORT friend Vector4 operator-(const float &left, const Vector4 &right);

	ACID_EXPORT friend Vector4 operator*(const float &left, const Vector4 &right);

	ACID_EXPORT friend Vector4 operator/(const float &left, const Vector4 &right);

	ACID_EXPORT friend Vector4 operator+(const Vector4 &left, const float &right);

	ACID_EXPORT friend Vector4 operator-(const Vector4 &left, const float &right);

	ACID_EXPORT friend Vector4 operator*(const Vector4 &left, const float &right);

	ACID_EXPORT friend Vector4 operator/(const Vector4 &left, const float &right);

	Vector4 &operator+=(const Vector4 &other);

	Vector4 &operator-=(const Vector4 &other);

	Vector4 &operator*=(const Vector4 &other);

	Vector4 &operator/=(const Vector4 &other);

	Vector4 &operator+=(const float &other);

	Vector4 &operator-=(const float &other);

	Vector4 &operator*=(const float &other);

	Vector4 &operator/=(const float &other);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Vector4 &vector);

	std::string ToString() const;

	static const Vector4 Zero;
	static const Vector4 One;
	static const Vector4 PositiveInfinity;
	static const Vector4 NegativeInfinity;

	union
	{
		struct
		{
			float m_elements[4];
		};

		struct
		{
			float m_x, m_y, m_z, m_w;
		};
	};
};
}

namespace std
{
template<>
struct hash<acid::Vector4>
{
	size_t operator()(acid::Vector4 const &vector) const
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
