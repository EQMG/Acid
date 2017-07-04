#pragma once

#include <iostream>
#include <cmath>

#include "vector2.h"

namespace flounder {
	/// <summary>
	/// Holds a 2x2 matrix.
	/// </summary>
	class matrix2x2
	{
	public:
		float m00, m01;
		float m10, m11;

		/// <summary>
		/// Constructor for matrix2x2. The matrix is initialised to the identity.
		/// </summary>
		matrix2x2();

		/// <summary>
		/// Constructor for matrix2x2.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a existing one. </param>
		matrix2x2(matrix2x2 *source);

		/// <summary>
		/// Constructor for matrix2x2.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 4 element array. </param>
		matrix2x2(float source[4]);

		/// <summary>
		/// Deconstructor for matrix2x2.
		/// </summary>
		~matrix2x2();

		/// <summary>
		/// Loads from another matrix2x2.
		/// </summary>
		/// <param name="source"> The source matrix.
		/// </param>
		/// <returns> This. </returns>
		matrix2x2 *set(matrix2x2 *source);

		/// <summary>
		/// Loads from a 4 element array.
		/// </summary>
		/// <param name="source"> The source array.
		/// </param>
		/// <returns> This. </returns>
		matrix2x2 *set(float source[4]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// </param>
		/// <returns> The source matrix. </returns>
		static matrix2x2 *setIdentity(matrix2x2 *source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix2x2 *add(matrix2x2 *left, matrix2x2 *right, matrix2x2 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix2x2 *subtract(matrix2x2 *left, matrix2x2 *right, matrix2x2 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix2x2 *multiply(matrix2x2 *left, matrix2x2 *right, matrix2x2 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector2 *transform(matrix2x2 *left, vector2 *right, vector2 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix2x2 *scale(matrix2x2 *left, vector2 *right, matrix2x2 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or null if a new one is to be created.
		/// </param>
		/// <returns> The inverted matrix, or null if source can't be reverted. </returns>
		static matrix2x2 *invert(matrix2x2 *source, matrix2x2 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The negated matrix. </returns>
		static matrix2x2 *negate(matrix2x2 *source, matrix2x2 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The transposed matrix. </returns>
		static matrix2x2 *transpose(matrix2x2 *source, matrix2x2 *destination);

		/// <summary>
		/// Turns a 2x2 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array.
		/// </param>
		/// <returns> A 4 float array. </returns>
		static float *toArray(matrix2x2 *matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0.
		/// </param>
		/// <returns> The matrix set to zero. </returns>
		static matrix2x2 *setZero(matrix2x2 *source);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix2x2 *setIdentity();

		/// <returns> The determinant of the matrix. </returns>
		float determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix2x2 *invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix2x2 *negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> this. </returns>
		matrix2x2 *transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> this. </returns>
		matrix2x2 *setZero();
	};
}
