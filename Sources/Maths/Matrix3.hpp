#pragma once

#include "Vector3.hpp"

namespace acid
{
class Matrix2;
class Matrix4;

/**
 * @brief Holds a row major 3x3 matrix.
 **/
class ACID_EXPORT Matrix3
{
public:
	/**
	 * Constructor for Matrix3. The matrix is initialised to the identity.
	 * @param diagonal The value set to the diagonals. 
	 **/
	explicit Matrix3(const float &diagonal = 1.0f);

	/**
	 * Constructor for Matrix3.
	 * @param source Creates this matrix out of a existing one. 
	 **/
	Matrix3(const Matrix2 &source);

	/**
	 * Constructor for Matrix3.
	 * @param source Creates this matrix out of a existing one. 
	 **/
	Matrix3(const Matrix4 &source);

	/**
	 * Constructor for Matrix3.
	 * @param source Creates this matrix out of a 9 element array. 
	 **/
	explicit Matrix3(const float source[9]);

	/**
	 * Constructor for Matrix3.
	 * @param source Creates this matrix out of a 3 vector array. 
	 **/
	explicit Matrix3(const Vector3f source[3]);

	/**
	 * Adds this matrix to another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix3 Add(const Matrix3 &other) const;

	/**
	 * Subtracts this matrix to another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix3 Subtract(const Matrix3 &other) const;

	/**
	 * Multiplies this matrix by another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix3 Multiply(const Matrix3 &other) const;

	/**
	 * Multiplies this matrix by a vector.
	 * @param other The vector. 
	 * @return The resultant vector. 
	 **/
	Vector3f Multiply(const Vector3f &other) const;

	/**
	 * Divides this matrix by another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix3 Divide(const Matrix3 &other) const;

	/**
	 * Transforms this matrix by a vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector3f Transform(const Vector3f &other) const;

	/**
	 * Scales this matrix by a vector.
	 * @param other The other vector. 
	 * @return The resultant matrix. 
	 **/
	Matrix3 Scale(const Vector3f &other) const;

	/**
	 * Inverts this matrix.
	 * @return The inverted matrix. 
	 **/
	Matrix3 Negate() const;

	/**
	 * Inverses this matrix.
	 * @return The inversed matrix. 
	 **/
	Matrix3 Inverse() const;

	/**
	 * Transposes this matrix.
	 * @return The transposed matrix. 
	 **/
	Matrix3 Transpose() const;

	/**
	 * Takes the determinant of this matrix.
	 * @return The determinant. 
	 **/
	float Determinant() const;

	/**
	 * Gets the submatrix of this matrix.
	 * @return The submatrix. 
	 **/
	Matrix2 GetSubmatrix(const int32_t &row, const int32_t &col) const;

	std::string ToString() const;

	bool operator==(const Matrix3 &other) const;

	bool operator!=(const Matrix3 &other) const;

	Matrix3 operator-() const;

	const Vector3f &operator[](const uint32_t &index) const;

	Vector3f &operator[](const uint32_t &index);

	ACID_EXPORT friend Matrix3 operator+(const Matrix3 &left, const Matrix3 &right);

	ACID_EXPORT friend Matrix3 operator-(const Matrix3 &left, const Matrix3 &right);

	ACID_EXPORT friend Matrix3 operator*(const Matrix3 &left, const Matrix3 &right);

	ACID_EXPORT friend Matrix3 operator/(const Matrix3 &left, const Matrix3 &right);

	ACID_EXPORT friend Matrix3 operator*(const Vector3f &left, const Matrix3 &right);

	ACID_EXPORT friend Matrix3 operator/(const Vector3f &left, const Matrix3 &right);

	ACID_EXPORT friend Matrix3 operator*(const Matrix3 &left, const Vector3f &right);

	ACID_EXPORT friend Matrix3 operator/(const Matrix3 &left, const Vector3f &right);

	ACID_EXPORT friend Matrix3 operator*(const float &left, const Matrix3 &right);

	ACID_EXPORT friend Matrix3 operator/(const float &left, const Matrix3 &right);

	ACID_EXPORT friend Matrix3 operator*(const Matrix3 &left, const float &right);

	ACID_EXPORT friend Matrix3 operator/(const Matrix3 &left, const float &right);

	Matrix3 &operator+=(const Matrix3 &other);

	Matrix3 &operator-=(const Matrix3 &other);

	Matrix3 &operator*=(const Matrix3 &other);

	Matrix3 &operator/=(const Matrix3 &other);

	Matrix3 &operator*=(const Vector3f &other);

	Matrix3 &operator/=(const Vector3f &other);

	Matrix3 &operator*=(const float &other);

	Matrix3 &operator/=(const float &other);

	ACID_EXPORT friend const Metadata& operator>>(const Metadata& metadata, Matrix3& matrix);

	ACID_EXPORT friend Metadata& operator<<(Metadata& metadata, const Matrix3& matrix);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Matrix3 &matrix);

	static const Matrix3 Identity;
	static const Matrix3 Zero;

	union
	{
		struct
		{
			Vector3f m_rows[3];
		};

		struct
		{
			float m_linear[9];
		};
	};
};
}

namespace std
{
template<>
struct hash<acid::Matrix3>
{
	size_t operator()(acid::Matrix3 const &matrix) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, matrix[0]);
		acid::Maths::HashCombine(seed, matrix[1]);
		acid::Maths::HashCombine(seed, matrix[2]);
		return seed;
	}
};
}
