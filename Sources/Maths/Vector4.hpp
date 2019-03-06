#pragma once

#include <ostream>
#include <string>
#include "Engine/Exports.hpp"
#include "Serialized/Metadata.hpp"
#include "Maths.hpp"

namespace acid
{
	class Colour;
	class Vector2;
	class Vector3;

	/// <summary>
	/// Holds a 4-tuple vector.
	/// </summary>
	class ACID_EXPORT Vector4
	{
	public:
		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		Vector4(const float &x = 0.0f, const float &y = 0.0f, const float &z = 0.0f, const float &w = 1.0f);

		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		/// <param name="a"> The XY value. </param>
		/// <param name="b"> The ZW value. </param>
		Vector4(const Vector2 &a, const Vector2 &b);

		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>>
		/// <param name="w"> Start w. </param>
		Vector4(const Vector3 &source, const float &w = 1.0f);

		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing colour. </param>
		Vector4(const Colour &source);

		/// <summary>
		/// Adds this vector to another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector4 Add(const Vector4 &other) const;

		/// <summary>
		/// Subtracts this vector to another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector4 Subtract(const Vector4 &other) const;

		/// <summary>
		/// Multiplies this vector with another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector4 Multiply(const Vector4 &other) const;

		/// <summary>
		/// Divides this vector by another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector4 Divide(const Vector4 &other) const;

		/// <summary>
		/// Calculates the angle between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The angle, in radians. </returns>
		float Angle(const Vector4 &other) const;

		/// <summary>
		/// Calculates the dot product of the this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The dot product. </returns>
		float Dot(const Vector4 &other) const;

		/// <summary>
		/// Calculates the linear interpolation between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <param name="progression"> The progression. </param>
		/// <returns> Left lerp right. </returns>
		Vector4 Lerp(const Vector4 &other, const float &progression) const;

		/// <summary>
		/// Scales this vector by a scalar.
		/// </summary>
		/// <param name="scalar"> The scalar value. </param>
		/// <returns> The scaled vector. </returns>
		Vector4 Scale(const float &scalar) const;

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns> The negated vector. </returns>
		Vector4 Negate() const;

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> The normalized vector. </returns>
		Vector4 Normalize() const;

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
		float DistanceSquared(const Vector4 &other) const;

		/// <summary>
		/// Gets the between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The distance. </returns>
		float Distance(const Vector4 &other) const;

		/// <summary>
		/// Gets the vector distance between this vector and another vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The vector distance. </returns>
		Vector4 DistanceVector(const Vector4 &other) const;

		/// <summary>
		/// Gradually changes this vector to a target.
		/// </summary>
		/// <param name="target"> The target vector. </param>
		/// <param name="rate"> The rate to go from current to the target. </param>
		/// <returns> The changed vector. </returns>
		Vector4 SmoothDamp(const Vector4 &target, const Vector4 &rate) const;

		/// <summary>
		/// Gets the lowest vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <returns> The lowest vector. </returns>
		static Vector4 MinVector(const Vector4 &a, const Vector4 &b);

		/// <summary>
		/// Gets the maximum vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <returns> The maximum vector. </returns>
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
		size_t operator()(acid::Vector4 const &vector) const noexcept
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
