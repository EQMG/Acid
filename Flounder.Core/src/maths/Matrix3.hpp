#pragma once

#include "maths.hpp"
#include "Vector3.hpp"

namespace Flounder
{
	/// <summary>
	/// Holds a 2x2 matrix.
	/// </summary>
	class Matrix3
	{
	public:
		float m_00;
		float m_01;
		float m_02;
		float m_10;
		float m_11;
		float m_12;
		float m_20;
		float m_21;
		float m_22;

		/// <summary>
		/// Constructor for Matrix3. The matrix is initialised to the identity.
		/// </summary>
		Matrix3();

		/// <summary>
		/// Constructor for Matrix3.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a existing one. </param>
		Matrix3(const Matrix3 &source);

		/// <summary>
		/// Constructor for Matrix3.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 9 element array. </param>
		Matrix3(const float source[9]);

		/// <summary>
		/// Deconstructor for Matrix3.
		/// </summary>
		~Matrix3();

		/// <summary>
		/// Loads from another Matrix3.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <returns> This. </returns>
		Matrix3 *set(const Matrix3 &source);

		/// <summary>
		/// Loads from a 9 element array.
		/// </summary>
		/// <param name="source"> The source array. </param>
		/// <returns> This. </returns>
		Matrix3 *set(const float source[9]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity. </param>
		/// <returns> The source matrix. </returns>
		static Matrix3 *setIdentity(Matrix3 *source);

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// <returns> The determinant of the matrix. </returns>
		static float determinant(const Matrix3 &source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix3 *add(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix3 *subtract(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix3 *multiply(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *transform(const Matrix3 &left, const Vector3 &right, Vector3 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix3 *scale(const Matrix3 &left, const Vector3 &right, Matrix3 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or nullptr if a new one is to be created. </param>
		/// <returns> The inverted matrix, or nullptr if source can't be reverted. </returns>
		static Matrix3 *invert(const Matrix3 &source, Matrix3 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The negated matrix. </returns>
		static Matrix3 *negate(const Matrix3 &source, Matrix3 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transposed matrix. </returns>
		static Matrix3 *transpose(const Matrix3 &source, Matrix3 *destination);

		/// <summary>
		/// Turns a 3x3 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array. </param>
		/// <returns> A 9 float array. </returns>
		static float *toArray(const Matrix3 &matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0. </param>
		/// <returns> The matrix set to zero. </returns>
		static Matrix3 *setZero(Matrix3 *source);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *setIdentity();

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant of the matrix. </returns>
		float determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *setZero();
	};
}
