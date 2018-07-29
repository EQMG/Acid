#pragma once

#include <ostream>
#include <string>
#include "Matrix4.hpp"
#include "Vector3.hpp"

namespace acid
{
	class Vector3;

	/// <summary>
	/// A vector like object of the form w + xi + yj + zk, where w, x, y, z are real numbers and i, j, k are imaginary units.
	/// </summary>
	class ACID_EXPORT Quaternion
	{
	public:
		union
		{
			struct
			{
				float m_x, m_y, m_z, m_w;
			};

			struct
			{
				float m_elements[4];
			};
		};

		static const Quaternion ZERO;
		static const Quaternion ONE;
		static const Quaternion W_ONE;
		static const Quaternion POSITIVE_INFINITY;
		static const Quaternion NEGATIVE_INFINITY;

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		Quaternion();

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		Quaternion(const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="pitch"> Start pitch. </param>
		/// <param name="yaw"> Start yaw. </param>
		/// <param name="roll"> Start roll. </param>
		Quaternion(const float &pitch, const float &yaw, const float &roll);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="source"> Creates this quaternion out of a existing vector (pitch, yaw roll). </param>>
		Quaternion(const Vector3 &source);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Quaternion(const Quaternion &source);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing matrix. </param>
		Quaternion(const Matrix4 &source);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="axis"> The axis to create from. </param>
		/// <param name="angle"> The angle to rotate the angle around. </param>
		Quaternion(const Vector3 &axis, const float &angle);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="axisX"> The X axis. </param>
		/// <param name="axisY"> The Y axis. </param>
		/// <param name="axisZ"> The Z axis. </param>
		Quaternion(const Vector3 &axisX, const Vector3 &axisY, const Vector3 &axisZ);

		/// <summary>
		/// Deconstructor for Quaternion.
		/// </summary>
		~Quaternion();

		/// <summary>
		/// Adds this quaternion to another quaternion.
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <returns> The resultant quaternion. </returns>
		Quaternion Add(const Quaternion &other) const;

		/// <summary>
		/// Subtracts this quaternion to another quaternion.
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <returns> The resultant quaternion. </returns>
		Quaternion Subtract(const Quaternion &other) const;

		/// <summary>
		/// Multiplies this quaternion with another quaternion.
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <returns> The resultant quaternion. </returns>
		Quaternion Multiply(const Quaternion &other) const;

		/// <summary>
		/// Multiplies this quaternion with another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector3 Multiply(const Vector3 &other) const;

		/// <summary>
		/// Multiplies this quaternion with the inverse of another quaternion. The value of both argument quaternions is persevered (this = left * right^-1).
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <returns> The resultant quaternion. </returns>
		Quaternion MultiplyInverse(const Quaternion &other) const;

		/// <summary>
		/// Calculates the dot product of the this quaternion and another quaternion.
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <returns> The dot product. </returns>
		float Dot(const Quaternion &other) const;

		/// <summary>
		/// Calculates the slerp between this quaternion and another quaternion, they must be normalized!
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <param name="progression"> The progression. </param>
		/// <returns> Left slerp right. </returns>
		Quaternion Slerp(const Quaternion &other, const float &progression);

		/// <summary>
		/// Scales this quaternion by a scalar.
		/// </summary>
		/// <param name="scalar"> The scalar value. </param>
		/// <returns> The scaled quaternion. </returns>
		Quaternion Scale(const float &scalar) const;

		/// <summary>
		/// Negates this quaternion.
		/// </summary>
		/// <returns> The negated quaternion. </returns>
		Quaternion Negate() const;

		/// <summary>
		/// Normalizes this quaternion.
		/// </summary>
		/// <returns> The normalized quaternion. </returns>
		Quaternion Normalize() const;

		/// <summary>
		/// Gets the length squared of this quaternion.
		/// </summary>
		/// <returns> The length squared. </returns>
		float LengthSquared() const;

		/// <summary>
		/// Gets the length of this quaternion.
		/// </summary>
		/// <returns> The length. </returns>
		float Length() const;

		/// <summary>
		/// Gets the maximum value in this quaternion.
		/// </summary>
		/// <returns> The largest components. </returns>
		float MaxComponent() const;

		/// <summary>
		/// Gets the lowest value in this quaternion.
		/// </summary>
		/// <returns> The smallest components. </returns>
		float MinComponent() const;

		/// <summary>
		/// Converts this quaternion to a 4x4 matrix.
		/// </summary>
		/// <returns> The rotation matrix which represents the exact same rotation as this quaternion. </returns>
		Matrix4 ToMatrix() const;

		/// <summary>
		/// Converts this quaternion to a 3x3 matrix representing the exact same
		/// rotation as this quaternion.
		/// </summary>
		/// <returns> The rotation matrix which represents the exact same rotation as this quaternion. </returns>
		Matrix4 ToRotationMatrix() const;

		/// <summary>
		/// Converts this quaternion to euler angles.
		/// </summary>
		/// <returns> The euler angle representation of this quaternion. </returns>
		Vector3 ToEuler() const;

		/// <summary>
		/// Saves this quaternion into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		float GetX() const { return m_x; }

		void SetX(const float &x) { m_x = x; }

		float GetY() const { return m_y; }

		void SetY(const float &y) { m_y = y; }

		float GetZ() const { return m_z; }

		void SetZ(const float &z) { m_z = z; }

		float GetW() const { return m_w; }

		void SetW(const float &w) { m_w = w; }

		Quaternion &operator=(const Quaternion &other);

		Quaternion &operator=(const Vector3 &other);

		Quaternion &operator=(const Matrix4 &other);

		Quaternion &operator=(LoadedValue *value);

		bool operator==(const Quaternion &other) const;

		bool operator!=(const Quaternion &other) const;

		bool operator<(const Quaternion &other) const;

		bool operator<=(const Quaternion &other) const;

		bool operator>(const Quaternion &other) const;

		bool operator>=(const Quaternion &other) const;

		bool operator==(const float &value) const;

		bool operator!=(const float &value) const;

		Quaternion operator-() const;

		const float &operator[](const uint32_t &index) const;

		float &operator[](const uint32_t &index);

		ACID_EXPORT friend Quaternion operator+(const Quaternion &left, const Quaternion &right);

		ACID_EXPORT friend Quaternion operator-(const Quaternion &left, const Quaternion &right);

		ACID_EXPORT friend Quaternion operator*(const Quaternion &left, const Quaternion &right);

		ACID_EXPORT friend Vector3 operator*(const Vector3 &right, const Quaternion &left);

		ACID_EXPORT friend Vector3 operator*(const Quaternion &left, const Vector3 &right);

		ACID_EXPORT friend Quaternion operator*(const float &left, const Quaternion &right);

		ACID_EXPORT friend Quaternion operator*(const Quaternion &left, const float &right);

		Quaternion &operator*=(const Quaternion &other);

		Quaternion &operator*=(const float &other);

		ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Quaternion &quaternion);

		std::string ToString() const;
	};
}
