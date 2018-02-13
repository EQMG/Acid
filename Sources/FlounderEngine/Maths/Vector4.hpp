#pragma once

#include <ostream>
#include <string>
#include "../Prerequisites.hpp"

namespace Flounder
{
	class Colour;

	class Vector3;

	/// <summary>
	/// Holds a 4-tuple vector.
	/// </summary>
	class F_EXPORT Vector4
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

		static const Vector4 ZERO;
		static const Vector4 ONE;
		static const Vector4 POSITIVE_INFINITY;
		static const Vector4 NEGATIVE_INFINITY;

		/// <summary>
		/// Constructor for vector4.
		/// </summary>
		Vector4();

		/// <summary>
		/// Constructor for vector4.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		Vector4(const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Constructor for vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector4(const Vector3 &source);

		/// <summary>
		/// Constructor for vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector4(const Vector4 &source);

		/// <summary>
		/// Constructor for vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing colour. </param>
		Vector4(const Colour &source);

		/// <summary>
		/// Deconstructor for vector4.
		/// </summary>
		~Vector4();

		/// <summary>
		/// Sets values in the vector.
		/// </summary>
		/// <param name="x"> The new X value. </param>
		/// <param name="y"> The new Y value. </param>
		/// <param name="z"> The new Z value. </param>
		/// <param name="w"> The new W value. </param>
		/// <returns> This. </returns>
		Vector4 *Set(const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Loads from another Vector4.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Vector4 *Set(const Vector3 &source);

		/// <summary>
		/// Loads from another Vector4.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Vector4 *Set(const Vector4 &source);

		/// <summary>
		/// Adds two vectors together and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *Add(const Vector4 &left, const Vector4 &right, Vector4 *destination);

		/// <summary>
		/// Subtracts two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *Subtract(const Vector4 &left, const Vector4 &right, Vector4 *destination);

		/// <summary>
		/// Multiplies two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *Multiply(const Vector4 &left, const Vector4 &right, Vector4 *destination);

		/// <summary>
		/// Divides two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *Divide(const Vector4 &left, const Vector4 &right, Vector4 *destination);

		/// <summary>
		/// Calculates the angle between two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <returns> The angle between the two vectors, in radians. </returns>
		static float Angle(const Vector4 &left, const Vector4 &right);

		/// <summary>
		/// Calculates the dot product of the two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <returns> Left dot right. </returns>
		static float Dot(const Vector4 &left, const Vector4 &right);

		/// <summary>
		/// Scales a vector by a scalar and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="scalar"> The scalar value. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *Scale(const Vector4 &source, const float &scalar, Vector4 *destination);

		/// <summary>
		/// Negates a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *Negate(const Vector4 &source, Vector4 *destination);

		/// <summary>
		/// Normalizes a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *Normalize(const Vector4 &source, Vector4 *destination);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length of the vector. </returns>
		static float Length(const Vector4 &source);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length squared of the vector. </returns>
		static float LengthSquared(const Vector4 &source);

		/// <summary>
		/// Gets the maximum vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *MaxVector(const Vector4 &a, const Vector4 &b, Vector4 *destination);

		/// <summary>
		/// Gets the lowest vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *MinVector(const Vector4 &a, const Vector4 &b, Vector4 *destination);

		/// <summary>
		/// Gets the maximum value in a vector.
		/// </summary>
		/// <param name="vector"> The value to get the maximum value from. </param>
		/// <returns> The maximum value. </returns>
		static float MaxComponent(const Vector4 &vector);

		/// <summary>
		/// Gets the lowest value in a vector.
		/// </summary>
		/// <param name="vector"> The value to get the lowest value from. </param>
		/// <returns> The lowest value. </returns>
		static float MinComponent(const Vector4 &vector);

		/// <summary>
		/// Gets the distance between two points squared.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <returns> The squared distance between the two points. </returns>
		static float GetDistanceSquared(const Vector4 &point1, const Vector4 &point2);

		/// <summary>
		/// Gets the total distance between 2 vectors.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <returns> The total distance between the points. </returns>
		static float GetDistance(const Vector4 &point1, const Vector4 &point2);

		/// <summary>
		/// Gets the vector distance between 2 vectors.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The vector distance between the points. </returns>
		static Vector4 *GetVectorDistance(const Vector4 &point1, const Vector4 &point2, Vector4 *destination);

		/// <summary>
		/// Translates this vector.
		/// </summary>
		/// <param name="x"> The translation in x. </param>
		/// <param name="y"> the translation in y. </param>
		/// <param name="z"> the translation in z. </param>
		/// <param name="w"> the translation in w. </param>
		/// <returns> This. </returns>
		Vector4 *Translate(const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns> This. </returns>
		Vector4 *Negate();

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> This. </returns>
		Vector4 *Normalize();

		/// <summary>
		/// Scales this vector.
		/// </summary>
		/// <param name="scalar"> The scale factor. </param>
		/// <returns> This. </returns>
		Vector4 *Scale(const float &scalar);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length of the vector. </returns>
		float Length() const;

		/// <summary>
		/// Gets the length squared of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length squared of the vector. </returns>
		float LengthSquared() const;

		bool operator==(const Vector4 &other) const;

		bool operator!=(const Vector4 &other) const;

		bool operator<(const Vector4 &other) const;

		bool operator<=(const Vector4 &other) const;

		bool operator>(const Vector4 &other) const;

		bool operator>=(const Vector4 &other) const;

		bool operator==(const float &value) const;

		bool operator!=(const float &value) const;

		Vector4 &operator-();

		friend Vector4 operator+(Vector4 left, const Vector4 &right);

		friend Vector4 operator-(Vector4 left, const Vector4 &right);

		friend Vector4 operator*(Vector4 left, const Vector4 &right);

		friend Vector4 operator/(Vector4 left, const Vector4 &right);

		friend Vector4 operator+(Vector4 left, float value);

		friend Vector4 operator-(Vector4 left, float value);

		friend Vector4 operator*(Vector4 left, float value);

		friend Vector4 operator/(Vector4 left, float value);

		friend Vector4 operator+(float value, Vector4 left);

		friend Vector4 operator-(float value, Vector4 left);

		friend Vector4 operator*(float value, Vector4 left);

		friend Vector4 operator/(float value, Vector4 left);

		Vector4 &operator+=(const Vector4 &other);

		Vector4 &operator-=(const Vector4 &other);

		Vector4 &operator*=(const Vector4 &other);

		Vector4 &operator/=(const Vector4 &other);

		Vector4 &operator+=(float value);

		Vector4 &operator-=(float value);

		Vector4 &operator*=(float value);

		Vector4 &operator/=(float value);

		friend std::ostream &operator<<(std::ostream &stream, const Vector4 &vector);

		std::string ToString() const;
	};
}
