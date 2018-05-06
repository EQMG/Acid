#pragma once

#include <ostream>
#include <string>
#include "Engine/Exports.hpp"
#include "Files/LoadedValue.hpp"

namespace fl
{
	class Colour;
	class Vector3;

	/// <summary>
	/// Holds a 4-tuple vector.
	/// </summary>
	class FL_EXPORT Vector4
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
				float m_r, m_g, m_b, m_a;
			};

			struct
			{
				float m_elements[4];
			};
		};

		static const Vector4 ZERO;
		static const Vector4 ONE;
		static const Vector4 POSITIVE_INFINITY;
		static const Vector4 NEGATIVE_INFINITY;

		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		Vector4();

		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		Vector4(const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>>
		/// <param name="w"> Start w. </param>
		Vector4(const Vector3 &source, const float &w = 1.0f);

		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector4(const Vector4 &source);

		/// <summary>
		/// Constructor for Vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing colour. </param>
		Vector4(const Colour &source);

		/// <summary>
		/// Deconstructor for vector4.
		/// </summary>
		~Vector4();

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
		/// Saves this vector into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		Vector4 &operator=(const Vector4 &other);

		Vector4 &operator=(const Vector3 &other);

		Vector4 &operator=(const Colour &other);

		Vector4 &operator=(LoadedValue *value);

		bool operator==(const Vector4 &other) const;

		bool operator!=(const Vector4 &other) const;

		bool operator<(const Vector4 &other) const;

		bool operator<=(const Vector4 &other) const;

		bool operator>(const Vector4 &other) const;

		bool operator>=(const Vector4 &other) const;

		bool operator==(const float &value) const;

		bool operator!=(const float &value) const;

		Vector4 operator-();

		FL_EXPORT friend Vector4 operator+(Vector4 left, const Vector4 &right);

		FL_EXPORT friend Vector4 operator-(Vector4 left, const Vector4 &right);

		FL_EXPORT friend Vector4 operator*(Vector4 left, const Vector4 &right);

		FL_EXPORT friend Vector4 operator/(Vector4 left, const Vector4 &right);

		FL_EXPORT friend Vector4 operator+(Vector4 left, float value);

		FL_EXPORT friend Vector4 operator-(Vector4 left, float value);

		FL_EXPORT friend Vector4 operator*(Vector4 left, float value);

		FL_EXPORT friend Vector4 operator/(Vector4 left, float value);

		FL_EXPORT friend Vector4 operator+(float value, Vector4 left);

		FL_EXPORT friend Vector4 operator-(float value, Vector4 left);

		FL_EXPORT friend Vector4 operator*(float value, Vector4 left);

		FL_EXPORT friend Vector4 operator/(float value, Vector4 left);

		Vector4 &operator+=(const Vector4 &other);

		Vector4 &operator-=(const Vector4 &other);

		Vector4 &operator*=(const Vector4 &other);

		Vector4 &operator/=(const Vector4 &other);

		Vector4 &operator+=(float value);

		Vector4 &operator-=(float value);

		Vector4 &operator*=(float value);

		Vector4 &operator/=(float value);

		FL_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Vector4 &vector);

		std::string ToString() const;
	};
}
