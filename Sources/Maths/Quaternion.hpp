#pragma once

#include "Matrix4.hpp"
#include "Vector3.hpp"

namespace acid
{
/**
 * @brief A vector like object of the form w + xi + yj + zk, where w, x, y, z are real numbers and i, j, k are imaginary units.
 **/
class ACID_EXPORT Quaternion
{
public:
	/**
	 * Constructor for Quaternion.
	 * @param x Start x. 
	 * @param y Start y. 
	 * @param z Start z. 
	 * @param w Start w. 
	 **/
	explicit Quaternion(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f, const float &w = 1.0f);

	/**
	 * Constructor for Quaternion.
	 * @param source Creates this quaternion out of a existing vector (pitch, yaw roll).
	 * @param w Start w. 
	 **/
	explicit Quaternion(const Vector3f &source);

	/**
	 * Constructor for Quaternion.
	 * @param source Creates this vector out of a existing matrix. 
	 **/
	Quaternion(const Matrix4 &source);

	/**
	 * Constructor for Quaternion.
	 * @param axisX The X axis. 
	 * @param axisY The Y axis. 
	 * @param axisZ The Z axis. 
	 **/
	Quaternion(const Vector3f &axisX, const Vector3f &axisY, const Vector3f &axisZ);

	/**
	 * Adds this quaternion to another quaternion.
	 * @param other The other quaternion. 
	 * @return The resultant quaternion. 
	 **/
	Quaternion Add(const Quaternion &other) const;

	/**
	 * Subtracts this quaternion to another quaternion.
	 * @param other The other quaternion. 
	 * @return The resultant quaternion. 
	 **/
	Quaternion Subtract(const Quaternion &other) const;

	/**
	 * Multiplies this quaternion with another quaternion.
	 * @param other The other quaternion. 
	 * @return The resultant quaternion. 
	 **/
	Quaternion Multiply(const Quaternion &other) const;

	/**
	 * Multiplies this quaternion with another vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector3f Multiply(const Vector3f &other) const;

	/**
	 * Multiplies this quaternion with the inverse of another quaternion. The value of both argument quaternions is persevered (this = left * right^-1).
	 * @param other The other quaternion. 
	 * @return The resultant quaternion. 
	 **/
	Quaternion MultiplyInverse(const Quaternion &other) const;

	/**
	 * Calculates the dot product of the this quaternion and another quaternion.
	 * @param other The other quaternion. 
	 * @return The dot product. 
	 **/
	float Dot(const Quaternion &other) const;

	/**
	 * Calculates the slerp between this quaternion and another quaternion, they must be normalized!
	 * @param other The other quaternion. 
	 * @param progression The progression. 
	 * @return Left slerp right. 
	 **/
	Quaternion Slerp(const Quaternion &other, const float &progression) const;

	/**
	 * Scales this quaternion by a scalar.
	 * @param scalar The scalar value. 
	 * @return The scaled quaternion. 
	 **/
	Quaternion Scale(const float &scalar) const;

	/**
	 * Normalizes this quaternion.
	 * @return The normalized quaternion. 
	 **/
	Quaternion Normalize() const;

	/**
	 * Gets the length squared of this quaternion.
	 * @return The length squared. 
	 **/
	float LengthSquared() const;

	/**
	 * Gets the length of this quaternion.
	 * @return The length. 
	 **/
	float Length() const;

	/**
	 * Gets the maximum value in this quaternion.
	 * @return The largest components. 
	 **/
	float MaxComponent() const;

	/**
	 * Gets the lowest value in this quaternion.
	 * @return The smallest components. 
	 **/
	float MinComponent() const;

	/**
	 * Converts this quaternion to a 4x4 matrix.
	 * @return The rotation matrix which represents the exact same rotation as this quaternion. 
	 **/
	Matrix4 ToMatrix() const;

	/**
	 * Converts this quaternion to a 3x3 matrix representing the exact same
	 * rotation as this quaternion.
	 * @return The rotation matrix which represents the exact same rotation as this quaternion. 
	 **/
	Matrix4 ToRotationMatrix() const;

	/**
	 * Converts this quaternion to euler angles.
	 * @return The euler angle representation of this quaternion. 
	 **/
	Vector3f ToEuler() const;

	const float &GetX() const { return m_x; }

	void SetX(const float &x) { m_x = x; }

	const float &GetY() const { return m_y; }

	void SetY(const float &y) { m_y = y; }

	const float &GetZ() const { return m_z; }

	void SetZ(const float &z) { m_z = z; }

	const float &GetW() const { return m_w; }

	void SetW(const float &w) { m_w = w; }

	std::string ToString() const;

	bool operator==(const Quaternion &other) const;

	bool operator!=(const Quaternion &other) const;

	Quaternion operator-() const;

	const float &operator[](const uint32_t &index) const;

	float &operator[](const uint32_t &index);

	ACID_EXPORT friend Quaternion operator+(const Quaternion &left, const Quaternion &right);

	ACID_EXPORT friend Quaternion operator-(const Quaternion &left, const Quaternion &right);

	ACID_EXPORT friend Quaternion operator*(const Quaternion &left, const Quaternion &right);

	ACID_EXPORT friend Vector3f operator*(const Vector3f &right, const Quaternion &left);

	ACID_EXPORT friend Vector3f operator*(const Quaternion &left, const Vector3f &right);

	ACID_EXPORT friend Quaternion operator*(const float &left, const Quaternion &right);

	ACID_EXPORT friend Quaternion operator*(const Quaternion &left, const float &right);

	Quaternion &operator*=(const Quaternion &other);

	Quaternion &operator*=(const float &other);

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, Quaternion &quaternion);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const Quaternion &quaternion);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Quaternion &quaternion);

	static const Quaternion Zero;
	static const Quaternion One;
	static const Quaternion OneW;
	static const Quaternion PositiveInfinity;
	static const Quaternion NegativeInfinity;

	float m_x, m_y, m_z, m_w;
};
}

namespace std
{
template<>
struct hash<acid::Quaternion>
{
	size_t operator()(const acid::Quaternion &quaternion) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, quaternion.m_x);
		acid::Maths::HashCombine(seed, quaternion.m_y);
		acid::Maths::HashCombine(seed, quaternion.m_z);
		acid::Maths::HashCombine(seed, quaternion.m_w);
		return seed;
	}
};
}
