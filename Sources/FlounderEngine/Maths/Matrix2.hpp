#pragma once

#include <ostream>
#include <string>
#include "Vector2.hpp"

namespace Flounder
{
	/// <summary>
	/// Holds a 2x2 matrix.
	/// </summary>
	class Matrix2
	{
	public:
		union
		{
			struct
			{
				Vector2 *m_0;
				Vector2 *m_1;
			};

			struct
			{
				float m_00, m_01;
				float m_10, m_11;
			};

			struct
			{
				float m_elements[2][2];
			};
		};

		/// <summary>
		/// Constructor for Matrix2. The matrix is initialised to the identity.
		/// </summary>
		Matrix2();

		/// <summary>
		/// Constructor for Matrix2.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a existing one. </param>
		Matrix2(const Matrix2 &source);

		/// <summary>
		/// Constructor for Matrix2.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 4 element array. </param>
		Matrix2(const float source[4]);

		/// <summary>
		/// Deconstructor for Matrix2.
		/// </summary>
		~Matrix2();

		/// <summary>
		/// Loads from another Matrix2.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <returns> This. </returns>
		Matrix2 *Set(const Matrix2 &source);

		/// <summary>
		/// Loads from a 4 element array.
		/// </summary>
		/// <param name="source"> The source array. </param>
		/// <returns> This. </returns>
		Matrix2 *Set(const float source[4]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity. </param>
		/// <returns> The source matrix. </returns>
		static Matrix2 *SetIdentity(Matrix2 *source);

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// <returns> The determinant of the matrix. </returns>
		static float Determinant(const Matrix2 &source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix2 *Add(const Matrix2 &left, const Matrix2 &right, Matrix2 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix2 *Subtract(const Matrix2 &left, const Matrix2 &right, Matrix2 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix2 *Multiply(const Matrix2 &left, const Matrix2 &right, Matrix2 *destination);

		/// <summary>
		/// Divides two matrices from each other and places the result in the destination matrices.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix2 *Divide(const Matrix2 &left, const Matrix2 &right, Matrix2 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector2 *Transform(const Matrix2 &left, const Vector2 &right, Vector2 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix2 *Scale(const Matrix2 &left, const Vector2 &right, Matrix2 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or nullptr if a new one is to be created. </param>
		/// <returns> The inverted matrix, or nullptr if source can't be reverted. </returns>
		static Matrix2 *Invert(const Matrix2 &source, Matrix2 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The negated matrix. </returns>
		static Matrix2 *Negate(const Matrix2 &source, Matrix2 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transposed matrix. </returns>
		static Matrix2 *Transpose(const Matrix2 &source, Matrix2 *destination);

		/// <summary>
		/// Turns a 2x2 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array. </param>
		/// <returns> A 4 float array. </returns>
		static float *ToArray(const Matrix2 &matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0. </param>
		/// <returns> The matrix set to zero. </returns>
		static Matrix2 *SetZero(Matrix2 *source);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix2 *SetIdentity();

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant of the matrix. </returns>
		float Determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix2 *Invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix2 *Negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> This. </returns>
		Matrix2 *Transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> This. </returns>
		Matrix2 *SetZero();

		bool operator==(const Matrix2 &other) const;

		bool operator!=(const Matrix2 &other) const;

		Matrix2 &operator-();

		friend Matrix2 operator+(Matrix2 left, const Matrix2 &right);

		friend Matrix2 operator-(Matrix2 left, const Matrix2 &right);

		friend Matrix2 operator*(Matrix2 left, const Matrix2 &right);

		friend Matrix2 operator/(Matrix2 left, const Matrix2 &right);

		friend Matrix2 operator*(Matrix2 left, Vector2 value);

		friend Matrix2 operator/(Matrix2 left, Vector2 value);

		Matrix2 &operator+=(const Matrix2 &other);

		Matrix2 &operator-=(const Matrix2 &other);

		Matrix2 &operator*=(const Matrix2 &other);

		Matrix2 &operator/=(const Matrix2 &other);

		friend std::ostream &operator<<(std::ostream &stream, const Matrix2 &matrix);

		std::string ToString() const;
	};
}
