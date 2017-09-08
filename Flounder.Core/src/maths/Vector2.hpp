#pragma once

#include "maths.hpp"

namespace Flounder
{
	class Vector3;

	/// <summary>
	/// Holds a 2-tuple vector.
	/// </summary>
	class Vector2
	{
	public:
		float m_x, m_y;

		/// <summary>
		/// Constructor for vector2.
		/// </summary>
		Vector2();

		/// <summary>
		/// Constructor for vector2.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		Vector2(const float &x, const float &y);

		/// <summary>
		/// Constructor for vector2.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector2(const Vector2 &source);

		/// <summary>
		/// Constructor for vector2.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector2(const Vector3 &source);

		/// <summary>
		/// Deconstructor for vector2.
		/// </summary>
		~Vector2();

		/// <summary>
		/// Sets values in the vector.
		/// </summary>
		/// <param name="x"> The new X value. </param>
		/// <param name="y"> The new Y value. </param>
		/// <returns> This. </returns>
		Vector2 *Set(const float &x, const float &y);

		/// <summary>
		/// Loads from another vector2.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Vector2 *Set(const Vector2 &source);

		/// <summary>
		/// Loads from another vector2.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Vector2 *Set(const Vector3 &source);

		/// <summary>
		/// Adds two vectors together and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Add(const Vector2 &left, const Vector2 &right, Vector2 *destination);

		/// <summary>
		/// Subtracts two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Subtract(const Vector2 &left, const Vector2 &right, Vector2 *destination);

		/// <summary>
		/// Multiplies two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Multiply(const Vector2 &left, const Vector2 &right, Vector2 *destination);

		/// <summary>
		/// Divides two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Divide(const Vector2 &left, const Vector2 &right, Vector2 *destination);

		/// <summary>
		/// Calculates the angle between two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <returns> The angle between the two vectors, in radians. </returns>
		static float Angle(const Vector2 &left, const Vector2 &right);

		/// <summary>
		/// Calculates the dot product of the two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <returns> Left dot right. </returns>
		static float Dot(const Vector2 &left, const Vector2 &right);

		/// <summary>
		/// Scales a vector by a scalar and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="scalar"> The scalar value. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Scale(const Vector2 &source, const float &scalar, Vector2 *destination);

		/// <summary>
		/// Rotates a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="angle"> The angle to rotate by <b>in degrees</b>. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Rotate(const Vector2 &source, const float &angle, Vector2 *destination);

		/// <summary>
		/// Rotates a vector around a point and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="angle"> The angle to rotate by <b>in degrees</b>. </param>
		/// <param name="rotationAxis"> The point to rotate the vector around. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Rotate(const Vector2 &source, const float &angle, const Vector2 &rotationAxis, Vector2 *destination);

		/// <summary>
		/// Negates a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Negate(const Vector2 &source, Vector2 *destination);

		/// <summary>
		/// Normalizes a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Normalize(const Vector2 &source, Vector2 *destination);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length of the vector. </returns>
		static float Length(const Vector2 &source);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length squared of the vector. </returns>
		static float LengthSquared(const Vector2 &source);

		/// <summary>
		/// Gets the maximum vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *MaxVector(const Vector2 &a, const Vector2 &b, Vector2 *destination);

		/// <summary>
		/// Gets the lowest vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *MinVector(const Vector2 &a, const Vector2 &b, Vector2 *destination);

		/// <summary>
		/// Gets the maximum value in a vector.
		/// </summary>
		/// <param name="vector"> The value to get the maximum value from. </param>
		/// <returns> The maximum value. </returns>
		static float MaxComponent(const Vector2 &vector);

		/// <summary>
		/// Gets the lowest value in a vector.
		/// </summary>
		/// <param name="vector"> The value to get the lowest value from. </param>
		/// <returns> The lowest value. </returns>
		static float MinComponent(const Vector2 &vector);

		/// <summary>
		/// Gets the distance between two points squared.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <returns> The squared distance between the two points. </returns>
		static float GetDistanceSquared(const Vector2 &point1, const Vector2 &point2);

		/// <summary>
		/// Gets the total distance between 2 vectors.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <returns> The total distance between the points. </returns>
		static float GetDistance(const Vector2 &point1, const Vector2 &point2);

		/// <summary>
		/// Gets the vector distance between 2 vectors.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The vector distance between the points. </returns>
		static Vector2 *GetVectorDistance(const Vector2 &point1, const Vector2 &point2, Vector2 *destination);

		/// <summary>
		/// Gets if the pt (point) is in a triangle.
		/// </summary>
		/// <param name="point"> The point to check. </param>
		/// <param name="v1"> The first triangle vertex. </param>
		/// <param name="v2"> The second triangle vertex. </param>
		/// <param name="v3"> The third triangle vertex. </param>
		/// <returns> If the point is in a triangle. </returns>
		static bool PointInTriangle(const Vector2 &point, const Vector2 &v1, const Vector2 &v2, const Vector2 &v3);

		/// <summary>
		/// Translates this vector.
		/// </summary>
		/// <param name="x"> The translation in x. </param>
		/// <param name="y"> the translation in y. </param>
		/// <returns> This. </returns>
		Vector2 *Translate(const float &x, const float &y);

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns> This. </returns>
		Vector2 *Negate();

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> This. </returns>
		Vector2 *Normalize();

		/// <summary>
		/// Scales this vector.
		/// </summary>
		/// <param name="scalar"> The scale factor. </param>
		/// <returns> This. </returns>
		Vector2 *Scale(const float &scalar);

		/// <summary>
		/// Gets if all the components to the vector are zero.
		/// </summary>
		/// <returns> If the vector is zero. </returns>
		bool IsZero() const;

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
	};
}
