#pragma once

#include <ostream>
#include <string>
#include "Vector3.hpp"

namespace acid
{
	class Matrix2;

	class Metadata;

	/// <summary>
	/// Holds a row major 3x3 matrix.
	/// </summary>
	class ACID_EXPORT Matrix3
	{
	public:
		union
		{
			struct
			{
				Vector3 m_rows[3];
			};

			struct
			{
				float m_linear[9];
			};
		};

		static const Matrix3 IDENTITY;
		static const Matrix3 ZERO;

		/// <summary>
		/// Constructor for Matrix3. The matrix is initialised to the identity.
		/// </summary>
		/// <param name="diagonal"> The value set to the diagonals. </param>
		Matrix3(const float &diagonal = 1.0f);

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
		/// Constructor for Matrix3.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 3 vector array. </param>
		Matrix3(const Vector3 source[3]);

		~Matrix3();

		/// <summary>
		/// Adds this matrix to another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix3 Add(const Matrix3 &other) const;

		/// <summary>
		/// Subtracts this matrix to another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix3 Subtract(const Matrix3 &other) const;

		/// <summary>
		/// Multiplies this matrix by another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix3 Multiply(const Matrix3 &other) const;

		/// <summary>
		/// Multiplies this matrix by a vector.
		/// </summary>
		/// <param name="other"> The vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector3 Multiply(const Vector3 &other) const;

		/// <summary>
		/// Divides this matrix by another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix3 Divide(const Matrix3 &other) const;

		/// <summary>
		/// Transforms this matrix by a vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector3 Transform(const Vector3 &other) const;

		/// <summary>
		/// Scales this matrix by a vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix3 Scale(const Vector3 &other) const;

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> The inverted matrix. </returns>
		Matrix3 Negate() const;

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> The negated matrix. </returns>
		Matrix3 Invert() const;

		/// <summary>
		/// Transposes this matrix.
		/// </summary>
		/// <returns> The transposed matrix. </returns>
		Matrix3 Transpose() const;

		/// <summary>
		/// Takes the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant. </returns>
		float Determinant() const;

		/// <summary>
		/// Gets the submatrix of this matrix.
		/// </summary>
		/// <returns> The submatrix. </returns>
		Matrix2 GetSubmatrix(const int32_t &row, const int32_t &col) const;

		void Decode(const Metadata &metadata);

		void Encode(Metadata &metadata) const;

		bool operator==(const Matrix3 &other) const;

		bool operator!=(const Matrix3 &other) const;

		Matrix3 operator-() const;

		const Vector3 &operator[](const uint32_t &index) const;

		Vector3 &operator[](const uint32_t &index);

		ACID_EXPORT friend Matrix3 operator+(const Matrix3 &left, const Matrix3 &right);

		ACID_EXPORT friend Matrix3 operator-(const Matrix3 &left, const Matrix3 &right);

		ACID_EXPORT friend Matrix3 operator*(const Matrix3 &left, const Matrix3 &right);

		ACID_EXPORT friend Matrix3 operator/(const Matrix3 &left, const Matrix3 &right);

		ACID_EXPORT friend Matrix3 operator*(const Vector3 &left, const Matrix3 &right);

		ACID_EXPORT friend Matrix3 operator/(const Vector3 &left, const Matrix3 &right);

		ACID_EXPORT friend Matrix3 operator*(const Matrix3 &left, const Vector3 &right);

		ACID_EXPORT friend Matrix3 operator/(const Matrix3 &left, const Vector3 &right);

		ACID_EXPORT friend Matrix3 operator*(const float &left, const Matrix3 &right);

		ACID_EXPORT friend Matrix3 operator/(const float &left, const Matrix3 &right);

		ACID_EXPORT friend Matrix3 operator*(const Matrix3 &left, const float &right);

		ACID_EXPORT friend Matrix3 operator/(const Matrix3 &left, const float &right);

		Matrix3 &operator+=(const Matrix3 &other);

		Matrix3 &operator-=(const Matrix3 &other);

		Matrix3 &operator*=(const Matrix3 &other);

		Matrix3 &operator/=(const Matrix3 &other);

		Matrix3 &operator*=(const Vector3 &other);

		Matrix3 &operator/=(const Vector3 &other);

		Matrix3 &operator*=(const float &other);

		Matrix3 &operator/=(const float &other);

		ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Matrix3 &matrix);

		std::string ToString() const;
	};
}
