#pragma once

#include "maths.hpp"
#include "vector3.hpp"

namespace flounder
{
	/// <summary>
	/// Holds a 2x2 matrix.
	/// </summary>
	class matrix3x3
	{
	public:
		float m_00, m_01, m_02;
		float m_10, m_11, m_12;
		float m_20, m_21, m_22;

		/// <summary>
		/// Constructor for matrix3x3. The matrix is initialised to the identity.
		/// </summary>
		matrix3x3();

		/// <summary>
		/// Constructor for matrix3x3.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a existing one. </param>
		matrix3x3(const matrix3x3 &source);

		/// <summary>
		/// Constructor for matrix3x3.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 9 element array. </param>
		matrix3x3(const float source[9]);

		/// <summary>
		/// Deconstructor for matrix3x3.
		/// </summary>
		~matrix3x3();

		/// <summary>
		/// Loads from another matrix3x3.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <returns> This. </returns>
		matrix3x3 *set(const matrix3x3 &source);

		/// <summary>
		/// Loads from a 9 element array.
		/// </summary>
		/// <param name="source"> The source array. </param>
		/// <returns> This. </returns>
		matrix3x3 *set(const float source[9]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity. </param>
		/// <returns> The source matrix. </returns>
		static matrix3x3 *setIdentity(matrix3x3 *source);

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// <returns> The determinant of the matrix. </returns>
		static float determinant(const matrix3x3 &source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static matrix3x3 *add(const matrix3x3 &left, const matrix3x3 &right, matrix3x3 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static matrix3x3 *subtract(const matrix3x3 &left, const matrix3x3 &right, matrix3x3 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static matrix3x3 *multiply(const matrix3x3 &left, const matrix3x3 &right, matrix3x3 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *transform(const matrix3x3 &left, const vector3 &right, vector3 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static matrix3x3 *scale(const matrix3x3 &left, const vector3 &right, matrix3x3 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or nullptr if a new one is to be created. </param>
		/// <returns> The inverted matrix, or nullptr if source can't be reverted. </returns>
		static matrix3x3 *invert(const matrix3x3 &source, matrix3x3 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The negated matrix. </returns>
		static matrix3x3 *negate(const matrix3x3 &source, matrix3x3 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transposed matrix. </returns>
		static matrix3x3 *transpose(const matrix3x3 &source, matrix3x3 *destination);

		/// <summary>
		/// Turns a 3x3 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array. </param>
		/// <returns> A 9 float array. </returns>
		static float *toArray(const matrix3x3 &matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0. </param>
		/// <returns> The matrix set to zero. </returns>
		static matrix3x3 *setZero(matrix3x3 *source);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> this-> </returns>
		matrix3x3 *setIdentity();

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant of the matrix. </returns>
		float determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> this-> </returns>
		matrix3x3 *invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> this-> </returns>
		matrix3x3 *negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> this-> </returns>
		matrix3x3 *transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> this-> </returns>
		matrix3x3 *setZero();
	};
}
