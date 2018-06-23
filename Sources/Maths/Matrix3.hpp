#pragma once

#include <ostream>
#include <string>
#include "Vector3.hpp"

namespace fl
{
	/// <summary>
	/// Holds a 3x3 matrix.
	/// </summary>
	class FL_EXPORT Matrix3
	{
	public:
		union
		{
			struct
			{
				Vector3 *m_0;
				Vector3 *m_1;
				Vector3 *m_2;
			};

			struct
			{
				float m_00, m_01, m_02;
				float m_10, m_11, m_12;
				float m_20, m_21, m_22;
			};

			struct
			{
				float m_elements[3][3];
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
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> The identity matrix. </returns>
		Matrix3 SetZero();

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> The identity matrix. </returns>
		Matrix3 SetIdentity();

		/// <summary>
		/// Saves this matrix into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		Matrix3 &operator=(const Matrix3 &other);

		Matrix3 &operator=(const float array[9]);

		Matrix3 &operator=(LoadedValue *value);

		bool operator==(const Matrix3 &other) const;

		bool operator!=(const Matrix3 &other) const;

		Matrix3 operator-();

		FL_EXPORT friend Matrix3 operator+(Matrix3 left, const Matrix3 &right);

		FL_EXPORT friend Matrix3 operator-(Matrix3 left, const Matrix3 &right);

		FL_EXPORT friend Matrix3 operator*(Matrix3 left, const Matrix3 &right);

		FL_EXPORT friend Matrix3 operator/(Matrix3 left, const Matrix3 &right);

		FL_EXPORT friend Matrix3 operator*(Matrix3 left, Vector3 value);

		FL_EXPORT friend Matrix3 operator/(Matrix3 left, Vector3 value);

		FL_EXPORT friend Matrix3 operator*(Matrix3 left, float value);

		FL_EXPORT friend Matrix3 operator/(Matrix3 left, float value);

		FL_EXPORT friend Matrix3 operator*(float value, Matrix3 left);

		FL_EXPORT friend Matrix3 operator/(float value, Matrix3 left);

		Matrix3 &operator+=(const Matrix3 &other);

		Matrix3 &operator-=(const Matrix3 &other);

		Matrix3 &operator*=(const Matrix3 &other);

		Matrix3 &operator/=(const Matrix3 &other);

		FL_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Matrix3 &matrix);

		std::string ToString() const;
	};
}
