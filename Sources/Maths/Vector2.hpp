#pragma once

#include <ostream>
#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	class Vector3;
	class DataStream;
	class Metadata;

	/// <summary>
	/// Holds a 2-tuple vector.
	/// </summary>
	class ACID_EXPORT Vector2
	{
	public:
		/// <summary>
		/// Constructor for Vector2.
		/// </summary>
		Vector2();

		/// <summary>
		/// Constructor for Vector2.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		Vector2(const float &x, const float &y);

		/// <summary>
		/// Constructor for Vector2.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector2(const Vector2 &source);

		/// <summary>
		/// Constructor for Vector2.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector2(const Vector3 &source);

		/// <summary>
		/// Adds this vector to another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector2 Add(const Vector2 &other) const;

		/// <summary>
		/// Subtracts this vector to another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector2 Subtract(const Vector2 &other) const;

		/// <summary>
		/// Multiplies this vector with another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector2 Multiply(const Vector2 &other) const;

		/// <summary>
		/// Divides this vector by another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector2 Divide(const Vector2 &other) const;

		/// <summary>
		/// Calculates the angle between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The angle, in radians. </returns>
		float Angle(const Vector2 &other) const;

		/// <summary>
		/// Calculates the dot product of the this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The dot product. </returns>
		float Dot(const Vector2 &other) const;

		/// <summary>
		/// Calculates the linear interpolation between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <param name="progression"> The progression. </param>
		/// <returns> Left lerp right. </returns>
		Vector2 Lerp(const Vector2 &other, const float &progression) const;

		/// <summary>
		/// Scales this vector by a scalar.
		/// </summary>
		/// <param name="scalar"> The scalar value. </param>
		/// <returns> The scaled vector. </returns>
		Vector2 Scale(const float &scalar) const;

		/// <summary>
		/// Rotates this vector by a angle around the origin.
		/// </summary>
		/// <param name="angle"> The angle to rotate by, in radians. </param>
		/// <returns> The rotated vector. </returns>
		Vector2 Rotate(const float &angle) const;

		/// <summary>
		/// Rotates this vector by a angle around a rotation axis.
		/// </summary>
		/// <param name="angle"> The angle to rotate by, in radians. </param>
		/// <param name="rotationAxis"> The point to rotate the vector around. </param>
		/// <returns> The rotated vector. </returns>
		Vector2 Rotate(const float &angle, const Vector2 &rotationAxis) const;

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns> The negated vector. </returns>
		Vector2 Negate() const;

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> The normalized vector. </returns>
		Vector2 Normalize() const;

		/// <summary>
		/// Gets the length squared of this vector.
		/// </summary>
		/// <returns> The length squared. </returns>
		float LengthSquared() const;

		/// <summary>
		/// Gets the length of this vector.
		/// </summary>
		/// <returns> The length. </returns>
		float Length() const;

		/// <summary>
		/// Gets the maximum value in this vector.
		/// </summary>
		/// <returns> The largest components. </returns>
		float MaxComponent() const;

		/// <summary>
		/// Gets the lowest value in this vector.
		/// </summary>
		/// <returns> The smallest components. </returns>
		float MinComponent() const;

		/// <summary>
		/// Gets the distance between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The squared distance. </returns>
		float DistanceSquared(const Vector2 &other) const;

		/// <summary>
		/// Gets the between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The distance. </returns>
		float Distance(const Vector2 &other) const;

		/// <summary>
		/// Gets the vector distance between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The vector distance. </returns>
		Vector2 DistanceVector(const Vector2 &other) const;

		/// <summary>
		/// Gets if this vector is in a triangle.
		/// </summary>
		/// <param name="v1"> The first triangle vertex. </param>
		/// <param name="v2"> The second triangle vertex. </param>
		/// <param name="v3"> The third triangle vertex. </param>
		/// <returns> If this vector is in a triangle. </returns>
		bool InTriangle(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3) const;

		/// <summary>
		/// Gradually changes this vector to a target.
		/// </summary>
		/// <param name="target"> The target vector. </param>
		/// <param name="rate"> The rate to go from current to the target. </param>
		/// <returns> The changed vector. </returns>
		Vector2 SmoothDamp(const Vector2 &target, const Vector2 &rate) const;

		/// <summary>
		/// Converts from rectangular to spherical coordinates, this vector is in cartesian (x, y).
		/// </summary>
		/// <returns> The polar coordinates (radius, theta). </returns>
		Vector2 CartesianToPolar() const;

		/// <summary>
		/// Converts from spherical to rectangular coordinates, this vector is in polar (radius, theta).
		/// </summary>
		/// <returns> The cartesian coordinates (x, y). </returns>
		Vector2 PolarToCartesian() const;

		/// <summary>
		/// Gets the lowest vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <returns> The lowest vector. </returns>
		static Vector2 MinVector(const Vector2 &a, const Vector2 &b);

		/// <summary>
		/// Gets the maximum vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <returns> The maximum vector. </returns>
		static Vector2 MaxVector(const Vector2 &a, const Vector2 &b);

		float GetX() const { return m_x; }

		void SetX(const float &x) { m_x = x; }

		float GetY() const { return m_y; }

		void SetY(const float &y) { m_y = y; }

		void Decode(const Metadata &metadata);

		void Encode(Metadata &metadata) const;

		bool operator==(const Vector2 &other) const;

		bool operator!=(const Vector2 &other) const;

		bool operator<(const Vector2 &other) const;

		bool operator<=(const Vector2 &other) const;

		bool operator>(const Vector2 &other) const;

		bool operator>=(const Vector2 &other) const;

		bool operator==(const float &value) const;

		bool operator!=(const float &value) const;

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

		ACID_EXPORT friend DataStream &operator<<(DataStream &stream, const Vector2 &vector);

		ACID_EXPORT friend DataStream &operator>>(DataStream &stream, Vector2 &vector);

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
