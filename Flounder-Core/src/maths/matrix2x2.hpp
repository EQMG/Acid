#pragma once

#include "maths.hpp"
#include "vector2.hpp"

namespace Flounder
{
	/// <summary>
	/// Holds a 2x2 matrix.
	/// </summary>
	class matrix2x2
	{
	public:
		float m_00;
		float m_01;
		float m_10;
		float m_11;

		/// <summary>
		/// Constructor for matrix2x2. The matrix is initialised to the identity.
		/// </summary>
		matrix2x2();

		/// <summary>
		/// Constructor for matrix2x2.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a existing one. </param>
		matrix2x2(const matrix2x2 &source);

		/// <summary>
		/// Constructor for matrix2x2.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 4 element array. </param>
		matrix2x2(const float source[4]);

		/// <summary>
		/// Deconstructor for matrix2x2.
		/// </summary>
		~matrix2x2();

		/// <summary>
		/// Loads from another matrix2x2.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <returns> This. </returns>
		matrix2x2 *set(const matrix2x2 &source);

		/// <summary>
		/// Loads from a 4 element array.
		/// </summary>
		/// <param name="source"> The source array. </param>
		/// <returns> This. </returns>
		matrix2x2 *set(const float source[4]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity. </param>
		/// <returns> The source matrix. </returns>
		static matrix2x2 *setIdentity(matrix2x2 *source);

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// <returns> The determinant of the matrix. </returns>
		static float determinant(const matrix2x2 &source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static matrix2x2 *add(const matrix2x2 &left, const matrix2x2 &right, matrix2x2 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static matrix2x2 *subtract(const matrix2x2 &left, const matrix2x2 &right, matrix2x2 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static matrix2x2 *multiply(const matrix2x2 &left, const matrix2x2 &right, matrix2x2 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector2 *transform(const matrix2x2 &left, const vector2 &right, vector2 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static matrix2x2 *scale(const matrix2x2 &left, const vector2 &right, matrix2x2 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or nullptr if a new one is to be created. </param>
		/// <returns> The inverted matrix, or nullptr if source can't be reverted. </returns>
		static matrix2x2 *invert(const matrix2x2 &source, matrix2x2 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The negated matrix. </returns>
		static matrix2x2 *negate(const matrix2x2 &source, matrix2x2 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transposed matrix. </returns>
		static matrix2x2 *transpose(const matrix2x2 &source, matrix2x2 *destination);

		/// <summary>
		/// Turns a 2x2 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array. </param>
		/// <returns> A 4 float array. </returns>
		static float *toArray(const matrix2x2 &matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0. </param>
		/// <returns> The matrix set to zero. </returns>
		static matrix2x2 *setZero(matrix2x2 *source);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> This. </returns>
		matrix2x2 *setIdentity();

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant of the matrix. </returns>
		float determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> This. </returns>
		matrix2x2 *invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> This. </returns>
		matrix2x2 *negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> This. </returns>
		matrix2x2 *transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> This. </returns>
		matrix2x2 *setZero();
	};
}
