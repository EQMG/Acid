#pragma once

#include <iostream>
#include <cmath>

#include "vector3.h"

namespace flounder {
	/// <summary>
	/// Holds a 2x2 matrix.
	/// </summary>
	class matrix3x3
	{
	public:
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;

		/// <summary>
		/// Constructor for matrix3x3. The matrix is initialised to the identity.
		/// </summary>
		matrix3x3();

		/// <summary>
		/// Constructor for matrix3x3.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a existing one. </param>
		matrix3x3(matrix3x3 *source);

		/// <summary>
		/// Constructor for matrix3x3.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 9 element array. </param>
		matrix3x3(float source[9]);

		/// <summary>
		/// Deconstructor for matrix3x3.
		/// </summary>
		~matrix3x3();

		/// <summary>
		/// Loads from another matrix3x3.
		/// </summary>
		/// <param name="source"> The source matrix.
		/// </param>
		/// <returns> This. </returns>
		matrix3x3 *set(matrix3x3 *source);

		/// <summary>
		/// Loads from a 9 element array.
		/// </summary>
		/// <param name="source"> The source array.
		/// </param>
		/// <returns> This. </returns>
		matrix3x3 *set(float source[9]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// </param>
		/// <returns> The source matrix. </returns>
		static matrix3x3 *setIdentity(matrix3x3 *source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix3x3 *add(matrix3x3 *left, matrix3x3 *right, matrix3x3 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix3x3 *subtract(matrix3x3 *left, matrix3x3 *right, matrix3x3 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix3x3 *multiply(matrix3x3 *left, matrix3x3 *right, matrix3x3 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector3 *transform(matrix3x3 *left, vector3 *right, vector3 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix3x3 *scale(matrix3x3 *left, vector3 *right, matrix3x3 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or null if a new one is to be created.
		/// </param>
		/// <returns> The inverted matrix, or null if source can't be reverted. </returns>
		static matrix3x3 *invert(matrix3x3 *source, matrix3x3 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The negated matrix. </returns>
		static matrix3x3 *negate(matrix3x3 *source, matrix3x3 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The transposed matrix. </returns>
		static matrix3x3 *transpose(matrix3x3 *source, matrix3x3 *destination);

		/// <summary>
		/// Turns a 3x3 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array.
		/// </param>
		/// <returns> A 9 float array. </returns>
		static float *toArray(matrix3x3 *matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0.
		/// </param>
		/// <returns> The matrix set to zero. </returns>
		static matrix3x3 *setZero(matrix3x3 *source);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix3x3 *setIdentity();

		/// <returns> The determinant of the matrix. </returns>
		float determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix3x3 *invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix3x3 *negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> this. </returns>
		matrix3x3 *transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> this. </returns>
		matrix3x3 *setZero();
	};
}
