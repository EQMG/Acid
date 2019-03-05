#pragma once

#include <ostream>
#include <string>
#include "Serialized/Metadata.hpp"
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
		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		Quaternion(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f, const float &w = 1.0f);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="source"> Creates this quaternion out of a existing vector (pitch, yaw roll). </param>>
		/// <param name="w"> Start w. </param>
		Quaternion(const Vector3 &source, const float &w = 1.0f);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing matrix. </param>
		Quaternion(const Matrix4 &source);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="axisX"> The X axis. </param>
		/// <param name="axisY"> The Y axis. </param>
		/// <param name="axisZ"> The Z axis. </param>
		Quaternion(const Vector3 &axisX, const Vector3 &axisY, const Vector3 &axisZ);

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
		Quaternion Slerp(const Quaternion &other, const float &progression) const;

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

		const float &GetX() const { return m_x; }

		void SetX(const float &x) { m_x = x; }

		const float &GetY() const { return m_y; }

		void SetY(const float &y) { m_y = y; }

		const float &GetZ() const { return m_z; }

		void SetZ(const float &z) { m_z = z; }

		const float &GetW() const { return m_w; }

		void SetW(const float &w) { m_w = w; }

		void Decode(const Metadata &metadata);

		void Encode(Metadata &metadata) const;

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

		static const Quaternion Zero;
		static const Quaternion One;
		static const Quaternion OneW;
		static const Quaternion PositiveInfinity;
		static const Quaternion NegativeInfinity;

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
	};
}

namespace std
{
	template<>
	struct hash<acid::Quaternion>
	{
		size_t operator()(acid::Quaternion const &quaternion) const noexcept
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
