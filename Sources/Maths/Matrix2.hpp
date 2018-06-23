#pragma once

#include <ostream>
#include <string>
#include "Vector2.hpp"

namespace fl
{
	/// <summary>
	/// Holds a 2x2 matrix.
	/// </summary>
	class FL_EXPORT Matrix2
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
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> The identity matrix. </returns>
		Matrix2 SetZero();

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> The identity matrix. </returns>
		Matrix2 SetIdentity();

		/// <summary>
		/// Saves this matrix into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		Matrix2 &operator=(const Matrix2 &other);

		Matrix2 &operator=(const float array[4]);

		Matrix2 &operator=(LoadedValue *value);

		bool operator==(const Matrix2 &other) const;

		bool operator!=(const Matrix2 &other) const;

		Matrix2 operator-();

		FL_EXPORT friend Matrix2 operator+(Matrix2 left, const Matrix2 &right);

		FL_EXPORT friend Matrix2 operator-(Matrix2 left, const Matrix2 &right);

		FL_EXPORT friend Matrix2 operator*(Matrix2 left, const Matrix2 &right);

		FL_EXPORT friend Matrix2 operator/(Matrix2 left, const Matrix2 &right);

		FL_EXPORT friend Matrix2 operator*(Matrix2 left, Vector2 value);

		FL_EXPORT friend Matrix2 operator/(Matrix2 left, Vector2 value);

		FL_EXPORT friend Matrix2 operator*(Matrix2 left, float value);

		FL_EXPORT friend Matrix2 operator/(Matrix2 left, float value);

		FL_EXPORT friend Matrix2 operator*(float value, Matrix2 left);

		FL_EXPORT friend Matrix2 operator/(float value, Matrix2 left);

		Matrix2 &operator+=(const Matrix2 &other);

		Matrix2 &operator-=(const Matrix2 &other);

		Matrix2 &operator*=(const Matrix2 &other);

		Matrix2 &operator/=(const Matrix2 &other);

		FL_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Matrix2 &matrix);

		std::string ToString() const;
	};
}
