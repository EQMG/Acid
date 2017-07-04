#pragma once

#include <iostream>
#include <cmath>

namespace flounder {
	/// <summary>
	/// Holds a 4-tuple vector.
	/// </summary>
	class vector4
	{
	public:
		float x, y, z, w;

		/// <summary>
		/// Constructor for vector4.
		/// </summary>
		vector4();

		/// <summary>
		/// Constructor for vector4.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		vector4(float x, float y, float z, float w);

		/// <summary>
		/// Constructor for vector4.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		vector4(vector4 *source);

		/// <summary>
		/// Deconstructor for vector4.
		/// </summary>
		~vector4();

		/// <summary>
		/// Sets values in the vector.
		/// </summary>
		/// <param name="x"> The new X value. </param>
		/// <param name="y"> The new Y value. </param>
		/// <param name="z"> The new Z value. </param>
		/// <param name="w"> The new W value. </param>
		/// <returns> This. </returns>
		vector4 *set(float x, float y, float z, float w);

		/// <summary>
		/// Loads from another vector4.
		/// </summary>
		/// <param name="source"> The source vector.
		/// </param>
		/// <returns> This. </returns>
		vector4 *set(vector4 *source);

		/// <summary>
		/// Adds two vectors together and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *add(vector4 *left, vector4 *right, vector4 *destination);

		/// <summary>
		/// Subtracts two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *subtract(vector4 *left, vector4 *right, vector4 *destination);

		/// <summary>
		/// Multiplies two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *multiply(vector4 *left, vector4 *right, vector4 *destination);

		/// <summary>
		/// Divides two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *divide(vector4 *left, vector4 *right, vector4 *destination);

		/// <summary>
		/// Calculates the angle between two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector.
		/// </param>
		/// <returns> The angle between the two vectors, in radians. </returns>
		static float angle(vector4 *left, vector4 *right);

		/// <summary>
		/// Calculates the dot product of the two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector.
		/// </param>
		/// <returns> Left dot right. </returns>
		static float dot(vector4 *left, vector4 *right);

		/// <summary>
		/// Scales a vector by a scalar and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="scalar"> The scalar value. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *scale(vector4 *source, float scalar, vector4 *destination);

		/// <summary>
		/// Negates a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *negate(vector4 *source, vector4 *destination);

		/// <summary>
		/// Normalizes a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *normalize(vector4 *source, vector4 *destination);

		/// <summary>
		/// Translates this vector.
		/// </summary>
		/// <param name="x"> The translation in x. </param>
		/// <param name="y"> the translation in y. </param>
		/// <param name="z"> the translation in z. </param>
		/// <param name="w"> the translation in w. </param>
		/// <returns> This. </returns>
		vector4 *translate(float x, float y, float z, float w);

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns> This. </returns>
		vector4 *negate();

		/// <summary>
		/// Normalises this vector.
		/// </summary>
		/// <returns> This. </returns>
		vector4 *normalize();

		/// <summary>
		/// Scales this vector.
		/// </summary>
		/// <param name="scalar"> The scale factor.
		/// </param>
		/// <returns> This. </returns>
		vector4 *scale(float scalar);

		/// <summary>
		/// Gets if all the components to the vector are zero.
		/// </summary>
		/// <returns> If the vector is zero. </returns>
		bool isZero();

		/// <returns> The length of the vector. </returns>
		float length();

		/// <returns> The length squared of the vector. </returns>
		float lengthSquared();
	};
}
