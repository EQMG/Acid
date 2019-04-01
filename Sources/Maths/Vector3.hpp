#pragma once

#include "Serialized/Metadata.hpp"
#include "Maths.hpp"

namespace acid
{
class Colour;
class Quaternion;
class Vector2;
class Vector4;

/**
 * @brief Holds a 3-tuple vector.
 **/
class ACID_EXPORT Vector3
{
public:
	/**
	 * Constructor for Vector3.
	 * @param x Start x. 
	 * @param y Start y. 
	 * @param z Start z. 
	 **/
	explicit Vector3(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f);

	/**
	 * Constructor for Vector3.
	 * @param source Creates this vector out of a existing one. 
	 * @param z Start z. 
	 **/
	Vector3(const Vector2 &source, const float &z = 0.0f);

	/**
	 * Constructor for Vector3.
	 * @param source Creates this vector out of a existing one. 
	 **/
	Vector3(const Vector4 &source);

	/**
	 * Constructor for Vector3.
	 * @param source Creates this vector out of a existing colour. 
	 **/
	Vector3(const Colour &source);

	/**
	 * Adds this vector to another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector3 Add(const Vector3 &other) const;

	/**
	 * Subtracts this vector to another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector3 Subtract(const Vector3 &other) const;

	/**
	 * Multiplies this vector with another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector3 Multiply(const Vector3 &other) const;

	/**
	 * Divides this vector by another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector3 Divide(const Vector3 &other) const;

	/**
	 * Calculates the angle between this vector and another vector.
	 * @param other The other vector. 
	 * @return The angle, in radians. 
	 **/
	float Angle(const Vector3 &other) const;

	/**
	 * Calculates the dot product of the this vector and another vector.
	 * @param other The other vector. 
	 * @return The dot product. 
	 **/
	float Dot(const Vector3 &other) const;

	/**
	 * Calculates the cross product of the this vector and another vector.
	 * @param other The other vector. 
	 * @return The cross product. 
	 **/
	Vector3 Cross(const Vector3 &other) const;

	/**
	 * Calculates the linear interpolation between this vector and another vector.
	 * @param other The other quaternion. 
	 * @param progression The progression. 
	 * @return Left lerp right. 
	 **/
	Vector3 Lerp(const Vector3 &other, const float &progression) const;

	/**
	 * Scales this vector by a scalar.
	 * @param scalar The scalar value. 
	 * @return The scaled vector. 
	 **/
	Vector3 Scale(const float &scalar) const;

	/**
	 * Rotates this vector by a angle around the origin.
	 * @param rotation The rotation amount. 
	 * @return The rotated vector. 
	 **/
	Vector3 Rotate(const Vector3 &rotation) const;

	/**
	 * Negates this vector.
	 * @return The negated vector. 
	 **/
	Vector3 Negate() const;

	/**
	 * Normalizes this vector.
	 * @return The normalized vector. 
	 **/
	Vector3 Normalize() const;

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
	 * Converts these euler angles to a quaternion.
	 * @return The quaternion representation of this vector. 
	 **/
	Quaternion ToQuaternion() const;

	/**
	 * Gets the distance between this vector and another vector.
	 * @param other The other vector. 
	 * @return The squared distance. 
	 **/
	float DistanceSquared(const Vector3 &other) const;

	/**
	 * Gets the between this vector and another vector.
	 * @param other The other vector. 
	 * @return The distance. 
	 **/
	float Distance(const Vector3 &other) const;

	/**
	 * Gets the vector distance between this vector and another vector.
	 * @param other The other vector. 
	 * @return The vector distance. 
	 **/
	Vector3 DistanceVector(const Vector3 &other) const;

	/**
	 * Gradually changes this vector to a target.
	 * @param target The target vector. 
	 * @param rate The rate to go from current to the target. 
	 * @return The changed vector. 
	 **/
	Vector3 SmoothDamp(const Vector3 &target, const Vector3 &rate) const;

	/**
	 * Converts from rectangular to spherical coordinates, this vector is in cartesian (x, y, z).
	 * @return The polar coordinates (radius, theta, phi). 
	 **/
	Vector3 CartesianToPolar() const;

	/**
	 * Converts from spherical to rectangular coordinates, this vector is in polar (radius, theta, phi).
	 * @return The cartesian coordinates (x, y, z). 
	 **/
	Vector3 PolarToCartesian() const;

	/**
	 * Projects this cube coordinate onto a sphere.
	 * @param radius The sphere radius. 
	 * @return The projected cartesian coordinates. 
	 **/
	Vector3 ProjectCubeToSphere(const float &radius) const;

	/**
	 * Gets the height of this vector on a point off of a 3d triangle.
	 * @param p1 Point 1 on the triangle. 
	 * @param p2 Point 2 on the triangle. 
	 * @param p3 Point 3 on the triangle. 
	 * @return Height of the triangle. 
	 **/
	float BaryCentric(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);

	/**
	 * Gets the lowest vector size.
	 * @param a The first vector to get values from. 
	 * @param b The second vector to get values from. 
	 * @return The lowest vector. 
	 **/
	static Vector3 MinVector(const Vector3 &a, const Vector3 &b);

	/**
	 * Gets the maximum vector size.
	 * @param a The first vector to get values from. 
	 * @param b The second vector to get values from. 
	 * @return The maximum vector. 
	 **/
	static Vector3 MaxVector(const Vector3 &a, const Vector3 &b);

	/**
	 * Generates a random unit vector.
	 * @return The random unit vector. 
	 **/
	static Vector3 RandomUnitVector();

	/**
	 * Gets a random point from on a circle.
	 * @param normal The circles normal. 
	 * @param radius The circles radius. 
	 * @return The random point in a circle. 
	 **/
	static Vector3 RandomPointOnCircle(const Vector3 &normal, const float &radius);

	/**
	 * Generates a random unit vector from within a cone.
	 * @param coneDirection The cones direction. 
	 * @param angle The cones major angle. 
	 * @return The random unitt vector in a code. 
	 **/
	static Vector3 RandomUnitVectorWithinCone(const Vector3 &coneDirection, const float &angle);

	float GetX() const { return m_x; }

	void SetX(const float &x) { m_x = x; }

	float GetY() const { return m_y; }

	void SetY(const float &y) { m_y = y; }

	float GetZ() const { return m_z; }

	void SetZ(const float &z) { m_z = z; }

	void Decode(const Metadata &metadata);

	void Encode(Metadata &metadata) const;

	bool operator==(const Vector3 &other) const;

	bool operator!=(const Vector3 &other) const;

	Vector3 operator-() const;

	const float &operator[](const uint32_t &index) const;

	float &operator[](const uint32_t &index);

	ACID_EXPORT friend Vector3 operator+(const Vector3 &left, const Vector3 &right);

	ACID_EXPORT friend Vector3 operator-(const Vector3 &left, const Vector3 &right);

	ACID_EXPORT friend Vector3 operator*(const Vector3 &left, const Vector3 &right);

	ACID_EXPORT friend Vector3 operator/(const Vector3 &left, const Vector3 &right);

	ACID_EXPORT friend Vector3 operator+(const float &left, const Vector3 &right);

	ACID_EXPORT friend Vector3 operator-(const float &left, const Vector3 &right);

	ACID_EXPORT friend Vector3 operator*(const float &left, const Vector3 &right);

	ACID_EXPORT friend Vector3 operator/(const float &left, const Vector3 &right);

	ACID_EXPORT friend Vector3 operator+(const Vector3 &left, const float &right);

	ACID_EXPORT friend Vector3 operator-(const Vector3 &left, const float &right);

	ACID_EXPORT friend Vector3 operator*(const Vector3 &left, const float &right);

	ACID_EXPORT friend Vector3 operator/(const Vector3 &left, const float &right);

	Vector3 &operator+=(const Vector3 &other);

	Vector3 &operator-=(const Vector3 &other);

	Vector3 &operator*=(const Vector3 &other);

	Vector3 &operator/=(const Vector3 &other);

	Vector3 &operator+=(const float &other);

	Vector3 &operator-=(const float &other);

	Vector3 &operator*=(const float &other);

	Vector3 &operator/=(const float &other);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Vector3 &vector);

	std::string ToString() const;

	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Left;
	static const Vector3 Right;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Front;
	static const Vector3 Back;
	static const Vector3 PositiveInfinity;
	static const Vector3 NegativeInfinity;

	union
	{
		struct
		{
			float m_elements[3];
		};

		struct
		{
			float m_x, m_y, m_z;
		};
	};
};
}

namespace std
{
template<>
struct hash<acid::Vector3>
{
	size_t operator()(acid::Vector3 const &vector) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, vector.m_x);
		acid::Maths::HashCombine(seed, vector.m_y);
		acid::Maths::HashCombine(seed, vector.m_z);
		return seed;
	}
};
}
