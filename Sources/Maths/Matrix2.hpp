#pragma once

#include "Vector2.hpp"

namespace acid
{
class Matrix3;
class Matrix4;

/**
 * @brief Holds a row major 2x2 matrix.
 **/
class ACID_EXPORT Matrix2
{
public:
	/**
	 * Constructor for Matrix2. The matrix is initialised to the identity.
	 * @param diagonal The value set to the diagonals. 
	 **/
	explicit Matrix2(const float &diagonal = 1.0f);

	/**
	 * Constructor for Matrix2.
	 * @param source Creates this matrix out of a existing one. 
	 **/
	Matrix2(const Matrix3 &source);

	/**
	 * Constructor for Matrix2.
	 * @param source Creates this matrix out of a existing one. 
	 **/
	Matrix2(const Matrix4 &source);

	/**
	 * Constructor for Matrix2.
	 * @param source Creates this matrix out of a 4 element array. 
	 **/
	explicit Matrix2(const float source[4]);

	/**
	 * Constructor for Matrix2.
	 * @param source Creates this matrix out of a 2 vector array. 
	 **/
	explicit Matrix2(const Vector2f source[2]);

	/**
	 * Adds this matrix to another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix2 Add(const Matrix2 &other) const;

	/**
	 * Subtracts this matrix to another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix2 Subtract(const Matrix2 &other) const;

	/**
	 * Multiplies this matrix by another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix2 Multiply(const Matrix2 &other) const;

	/**
	 * Divides this matrix by another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix2 Divide(const Matrix2 &other) const;

	/**
	 * Transforms this matrix by a vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector2f Transform(const Vector2f &other) const;

	/**
	 * Scales this matrix by a vector.
	 * @param other The other vector. 
	 * @return The resultant matrix. 
	 **/
	Matrix2 Scale(const Vector2f &other) const;

	/**
	 * Inverts this matrix.
	 * @return The inverted matrix. 
	 **/
	Matrix2 Negate() const;

	/**
	 * Inverses this matrix.
	 * @return The inversed matrix. 
	 **/
	Matrix2 Inverse() const;

	/**
	 * Transposes this matrix.
	 * @return The transposed matrix. 
	 **/
	Matrix2 Transpose() const;

	/**
	 * Takes the determinant of this matrix.
	 * @return The determinant. 
	 **/
	float Determinant() const;

	/**
	 * Gets the submatrix of this matrix.
	 * @return The submatrix. 
	 **/
	float GetSubmatrix(const int32_t &row, const int32_t &col) const;

	std::string ToString() const;

	bool operator==(const Matrix2 &other) const;

	bool operator!=(const Matrix2 &other) const;

	Matrix2 operator-() const;

	const Vector2f &operator[](const uint32_t &index) const;

	Vector2f &operator[](const uint32_t &index);

	ACID_EXPORT friend Matrix2 operator+(const Matrix2 &left, const Matrix2 &right);

	ACID_EXPORT friend Matrix2 operator-(const Matrix2 &left, const Matrix2 &right);

	ACID_EXPORT friend Matrix2 operator*(const Matrix2 &left, const Matrix2 &right);

	ACID_EXPORT friend Matrix2 operator/(const Matrix2 &left, const Matrix2 &right);

	ACID_EXPORT friend Matrix2 operator*(const Vector2f &left, const Matrix2 &right);

	ACID_EXPORT friend Matrix2 operator/(const Vector2f &left, const Matrix2 &right);

	ACID_EXPORT friend Matrix2 operator*(const Matrix2 &left, const Vector2f &right);

	ACID_EXPORT friend Matrix2 operator/(const Matrix2 &left, const Vector2f &right);

	ACID_EXPORT friend Matrix2 operator*(const float &left, const Matrix2 &right);

	ACID_EXPORT friend Matrix2 operator/(const float &left, const Matrix2 &right);

	ACID_EXPORT friend Matrix2 operator*(const Matrix2 &left, const float &right);

	ACID_EXPORT friend Matrix2 operator/(const Matrix2 &left, const float &right);

	Matrix2 &operator+=(const Matrix2 &other);

	Matrix2 &operator-=(const Matrix2 &other);

	Matrix2 &operator*=(const Matrix2 &other);

	Matrix2 &operator/=(const Matrix2 &other);

	Matrix2 &operator*=(const Vector2f &other);

	Matrix2 &operator/=(const Vector2f &other);

	Matrix2 &operator*=(const float &other);

	Matrix2 &operator/=(const float &other);

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, Matrix2 &matrix);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const Matrix2 &matrix);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Matrix2 &matrix);

	static const Matrix2 Identity;
	static const Matrix2 Zero;

	union
	{
		struct
		{
			Vector2f m_rows[2];
		};

		struct
		{
			float m_linear[4];
		};
	};
};
}

namespace std
{
template<>
struct hash<acid::Matrix2>
{
	size_t operator()(acid::Matrix2 const &matrix) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, matrix[0]);
		acid::Maths::HashCombine(seed, matrix[1]);
		return seed;
	}
};
}
