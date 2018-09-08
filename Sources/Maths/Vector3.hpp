#pragma once

#include <ostream>
#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	class Colour;

	class Quaternion;

	class Vector2;

	class Vector4;

	class Metadata;

	/// <summary>
	/// Holds a 3-tuple vector.
	/// </summary>
	class ACID_EXPORT Vector3
	{
	public:
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

		static const Vector3 ZERO;
		static const Vector3 ONE;
		static const Vector3 LEFT;
		static const Vector3 RIGHT;
		static const Vector3 UP;
		static const Vector3 DOWN;
		static const Vector3 FRONT;
		static const Vector3 BACK;
		static const Vector3 POSITIVE_INFINITY;
		static const Vector3 NEGATIVE_INFINITY;

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		Vector3();

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		Vector3(const float &x, const float &y, const float &z);

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		/// <param name="z"> Start z. </param>
		Vector3(const Vector2 &source, const float &z = 0.0f);

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector3(const Vector3 &source);

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector3(const Vector4 &source);

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing colour. </param>
		Vector3(const Colour &source);

		~Vector3();

		/// <summary>
		/// Adds this vector to another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector3 Add(const Vector3 &other) const;

		/// <summary>
		/// Subtracts this vector to another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector3 Subtract(const Vector3 &other) const;

		/// <summary>
		/// Multiplies this vector with another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector3 Multiply(const Vector3 &other) const;

		/// <summary>
		/// Divides this vector by another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector3 Divide(const Vector3 &other) const;

		/// <summary>
		/// Calculates the angle between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The angle, in radians. </returns>
		float Angle(const Vector3 &other) const;

		/// <summary>
		/// Calculates the dot product of the this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The dot product. </returns>
		float Dot(const Vector3 &other) const;

		/// <summary>
		/// Calculates the cross product of the this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The cross product. </returns>
		Vector3 Cross(const Vector3 &other) const;

		/// <summary>
		/// Scales this vector by a scalar.
		/// </summary>
		/// <param name="scalar"> The scalar value. </param>
		/// <returns> The scaled vector. </returns>
		Vector3 Scale(const float &scalar) const;

		/// <summary>
		/// Rotates this vector by a angle around the origin.
		/// </summary>
		/// <param name="rotation"> The rotation amount. </param>
		/// <returns> The rotated vector. </returns>
		Vector3 Rotate(const Vector3 &rotation) const;

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns> The negated vector. </returns>
		Vector3 Negate() const;

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> The normalized vector. </returns>
		Vector3 Normalize() const;

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
		/// Converts these euler angles to a quaternion.
		/// </summary>
		/// <returns> The quaternion representation of this vector. </returns>
		Quaternion ToQuaternion() const;

		/// <summary>
		/// Gets the distance between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The squared distance. </returns>
		float DistanceSquared(const Vector3 &other) const;

		/// <summary>
		/// Gets the between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The distance. </returns>
		float Distance(const Vector3 &other) const;

		/// <summary>
		/// Gets the vector distance between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The vector distance. </returns>
		Vector3 DistanceVector(const Vector3 &other) const;

		/// <summary>
		/// Gradually changes this vector to a target.
		/// </summary>
		/// <param name="target"> The target vector. </param>
		/// <param name="rate"> The rate to go from current to the target. </param>
		/// <returns> The changed vector. </returns>
		Vector3 SmoothDamp(const Vector3 &target, const Vector3 &rate) const;

		/// <summary>
		/// Converts from rectangular to spherical coordinates, this vector is in cartesian (x, y, z).
		/// </summary>
		/// <returns> The polar coordinates (radius, theta, phi). </returns>
		Vector3 CartesianToPolar() const;

		/// <summary>
		/// Converts from spherical to rectangular coordinates, this vector is in polar (radius, theta, phi).
		/// </summary>
		/// <returns> The cartesian coordinates (x, y, z). </returns>
		Vector3 PolarToCartesian() const;

		/// <summary>
		/// Projects this cube coordinate onto a sphere.
		/// </summary>
		/// <param name="radius"> The sphere radius. </param>
		/// <returns> The projected cartesian coordinates. </returns>
		Vector3 ProjectCubeToSphere(const float &radius) const;

		/// <summary>
		/// Gets the height of this vector on a point off of a 3d triangle.
		/// </summary>
		/// <param name="p1"> Point 1 on the triangle. </param>
		/// <param name="p2"> Point 2 on the triangle. </param>
		/// <param name="p3"> Point 3 on the triangle. </param>
		/// <returns> Height of the triangle. </returns>
		float BaryCentric(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);

		/// <summary>
		/// Gets the lowest vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <returns> The lowest vector. </returns>
		static Vector3 MinVector(const Vector3 &a, const Vector3 &b);

		/// <summary>
		/// Gets the maximum vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <returns> The maximum vector. </returns>
		static Vector3 MaxVector(const Vector3 &a, const Vector3 &b);

		/// <summary>
		/// Generates a random unit vector.
		/// </summary>
		/// <returns> The random unit vector. </returns>
		static Vector3 RandomUnitVector();

		/// <summary>
		/// Gets a random point from on a circle.
		/// </summary>
		/// <param name="normal"> The circles normal. </param>
		/// <param name="radius"> The circles radius. </param>
		/// <returns> The random point in a circle. </returns>
		static Vector3 RandomPointOnCircle(const Vector3 &normal, const float &radius);

		/// <summary>
		/// Generates a random unit vector from within a cone.
		/// </summary>
		/// <param name="coneDirection"> The cones direction. </param>
		/// <param name="angle"> The cones major angle. </param>
		/// <returns> The random unitt vector in a code. </returns>
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

		bool operator<(const Vector3 &other) const;

		bool operator<=(const Vector3 &other) const;

		bool operator>(const Vector3 &other) const;

		bool operator>=(const Vector3 &other) const;

		bool operator==(const float &value) const;

		bool operator!=(const float &value) const;

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
	};
}
