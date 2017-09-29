#pragma once

#include "Maths.hpp"
#include "Vector3.hpp"

namespace Flounder
{
	/// <summary>
	/// Holds a 2x2 matrix.
	/// </summary>
	class Matrix3
	{
	public:
		union
		{
			struct
			{
				float m_elements[3][3];
			};

			struct
			{
				float m_00, m_01, m_02;
				float m_10, m_11, m_12;
				float m_20, m_21, m_22;
			};

			struct
			{
				Vector3 m_0;
				Vector3 m_1;
				Vector3 m_2;
			};
		};

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
		Matrix3 *Set(const Matrix3 &source);

		/// <summary>
		/// Loads from a 9 element array.
		/// </summary>
		/// <param name="source"> The source array. </param>
		/// <returns> This. </returns>
		Matrix3 *Set(const float source[9]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity. </param>
		/// <returns> The source matrix. </returns>
		static Matrix3 *SetIdentity(Matrix3 *source);

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// <returns> The determinant of the matrix. </returns>
		static float Determinant(const Matrix3 &source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix3 *Add(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix3 *Subtract(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix3 *Multiply(const Matrix3 &left, const Matrix3 &right, Matrix3 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Transform(const Matrix3 &left, const Vector3 &right, Vector3 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix3 *Scale(const Matrix3 &left, const Vector3 &right, Matrix3 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or nullptr if a new one is to be created. </param>
		/// <returns> The inverted matrix, or nullptr if source can't be reverted. </returns>
		static Matrix3 *Invert(const Matrix3 &source, Matrix3 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The negated matrix. </returns>
		static Matrix3 *Negate(const Matrix3 &source, Matrix3 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transposed matrix. </returns>
		static Matrix3 *Transpose(const Matrix3 &source, Matrix3 *destination);

		/// <summary>
		/// Turns a 3x3 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array. </param>
		/// <returns> A 9 float array. </returns>
		static float *ToArray(const Matrix3 &matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0. </param>
		/// <returns> The matrix set to zero. </returns>
		static Matrix3 *SetZero(Matrix3 *source);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *SetIdentity();

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant of the matrix. </returns>
		float Determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *Invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *Negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *Transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> This. </returns>
		Matrix3 *SetZero();
	};
}
