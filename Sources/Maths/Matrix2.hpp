#pragma once

#include <ostream>
#include <string>
#include "Vector2.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	/// <summary>
	/// Holds a row major 2x2 matrix.
	/// </summary>
	class ACID_EXPORT Matrix2
	{
	public:
		union
		{
			struct
			{
				Vector2 m_rows[2];
			};

			struct
			{
				float m_linear[4];
			};
		};

		static const Matrix2 IDENTITY;
		static const Matrix2 ZERO;

		/// <summary>
		/// Constructor for Matrix2. The matrix is initialised to the identity.
		/// </summary>
		/// <param name="diagonal"> The value set to the diagonals. </param>
		Matrix2(const float &diagonal = 1.0f);

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
		/// Constructor for Matrix2.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 2 vector array. </param>
		Matrix2(const Vector2 source[2]);

		~Matrix2();

		/// <summary>
		/// Adds this matrix to another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix2 Add(const Matrix2 &other) const;

		/// <summary>
		/// Subtracts this matrix to another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix2 Subtract(const Matrix2 &other) const;

		/// <summary>
		/// Multiplies this matrix by another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix2 Multiply(const Matrix2 &other) const;

		/// <summary>
		/// Divides this matrix by another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix2 Divide(const Matrix2 &other) const;

		/// <summary>
		/// Transforms this matrix by a vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector2 Transform(const Vector2 &other) const;

		/// <summary>
		/// Scales this matrix by a vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix2 Scale(const Vector2 &other) const;

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> The inverted matrix. </returns>
		Matrix2 Negate() const;

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> The negated matrix. </returns>
		Matrix2 Invert() const;

		/// <summary>
		/// Transposes this matrix.
		/// </summary>
		/// <returns> The transposed matrix. </returns>
		Matrix2 Transpose() const;

		/// <summary>
		/// Takes the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant. </returns>
		float Determinant() const;

		/// <summary>
		/// Gets the submatrix of this matrix.
		/// </summary>
		/// <returns> The submatrix. </returns>
		float GetSubmatrix(const int32_t &row, const int32_t &col) const;

		void Decode(const Metadata &metadata);

		void Encode(Metadata &metadata) const;

		bool operator==(const Matrix2 &other) const;

		bool operator!=(const Matrix2 &other) const;

		Matrix2 operator-() const;

		const Vector2 &operator[](const uint32_t &index) const;

		Vector2 &operator[](const uint32_t &index);

		ACID_EXPORT friend Matrix2 operator+(const Matrix2 &left, const Matrix2 &right);

		ACID_EXPORT friend Matrix2 operator-(const Matrix2 &left, const Matrix2 &right);

		ACID_EXPORT friend Matrix2 operator*(const Matrix2 &left, const Matrix2 &right);

		ACID_EXPORT friend Matrix2 operator/(const Matrix2 &left, const Matrix2 &right);

		ACID_EXPORT friend Matrix2 operator*(const Vector2 &left, const Matrix2 &right);

		ACID_EXPORT friend Matrix2 operator/(const Vector2 &left, const Matrix2 &right);

		ACID_EXPORT friend Matrix2 operator*(const Matrix2 &left, const Vector2 &right);

		ACID_EXPORT friend Matrix2 operator/(const Matrix2 &left, const Vector2 &right);

		ACID_EXPORT friend Matrix2 operator*(const float &left, const Matrix2 &right);

		ACID_EXPORT friend Matrix2 operator/(const float &left, const Matrix2 &right);

		ACID_EXPORT friend Matrix2 operator*(const Matrix2 &left, const float &right);

		ACID_EXPORT friend Matrix2 operator/(const Matrix2 &left, const float &right);

		Matrix2 &operator+=(const Matrix2 &other);

		Matrix2 &operator-=(const Matrix2 &other);

		Matrix2 &operator*=(const Matrix2 &other);

		Matrix2 &operator/=(const Matrix2 &other);

		Matrix2 &operator*=(const Vector2 &other);

		Matrix2 &operator/=(const Vector2 &other);

		Matrix2 &operator*=(const float &other);

		Matrix2 &operator/=(const float &other);

		ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Matrix2 &matrix);

		std::string ToString() const;
	};
}
