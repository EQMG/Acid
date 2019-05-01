#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace acid
{
class Matrix2;
class Matrix3;

/**
 * @brief Holds a row major 4x4 matrix.
 **/
class ACID_EXPORT Matrix4
{
public:
	/**
	 * Constructor for Matrix4. The matrix is initialised to the identity.
	 * @param diagonal The value set to the diagonals. 
	 **/
	explicit Matrix4(const float &diagonal = 1.0f);

	/**
	 * Constructor for Matrix4.
	 * @param source Creates this matrix out of a existing one. 
	 **/
	Matrix4(const Matrix2 &source);

	/**
	 * Constructor for Matrix4.
	 * @param source Creates this matrix out of a existing one. 
	 **/
	Matrix4(const Matrix3 &source);

	/**
	 * Constructor for Matrix4.
	 * @param source Creates this matrix out of a 16 element array. 
	 **/
	explicit Matrix4(const float source[16]);

	/**
	 * Constructor for Matrix4.
	 * @param source Creates this matrix out of a 4 vector array. 
	 **/
	explicit Matrix4(const Vector4f source[4]);

	/**
	 * Adds this matrix to another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix4 Add(const Matrix4 &other) const;

	/**
	 * Subtracts this matrix to another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix4 Subtract(const Matrix4 &other) const;

	/**
	 * Multiplies this matrix by another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix4 Multiply(const Matrix4 &other) const;

	/**
	 * Multiplies this matrix by a vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector4f Multiply(const Vector4f &other) const;

	/**
	 * Divides this matrix by another matrix.
	 * @param other The other matrix. 
	 * @return The resultant matrix. 
	 **/
	Matrix4 Divide(const Matrix4 &other) const;

	/**
	 * Transforms this matrix by a vector.
	 * @param other The other vector. 
	 * @return The resultant vector. 
	 **/
	Vector4f Transform(const Vector4f &other) const;

	/**
	 * Translates this matrix by a vector.
	 * @param other The vector. 
	 * @return The resultant matrix. 
	 **/
	Matrix4 Translate(const Vector2f &other) const;

	/**
	 * Translates this matrix by a vector.
	 * @param other The vector. 
	 * @return The resultant matrix. 
	 **/
	Matrix4 Translate(const Vector3f &other) const;

	/**
	 * Scales this matrix by a vector.
	 * @param other The other vector. 
	 * @return The resultant matrix. 
	 **/
	Matrix4 Scale(const Vector3f &other) const;

	/**
	 * Scales this matrix by a vector.
	 * @param other The other vector. 
	 * @return The resultant matrix. 
	 **/
	Matrix4 Scale(const Vector4f &other) const;

	/**
	 * Rotates this matrix around the given axis the specified angle.
	 * @param angle The angle, in radians. 
	 * @param axis The vector representing the rotation axis. 
	 * @return The rotated matrix. 
	 **/
	Matrix4 Rotate(const float &angle, const Vector3f &axis) const;

	/**
	 * Inverts this matrix.
	 * @return The inverted matrix. 
	 **/
	Matrix4 Negate() const;

	/**
	 * Inverses this matrix.
	 * @return The inversed matrix. 
	 **/
	Matrix4 Inverse() const;

	/**
	 * Transposes this matrix.
	 * @return The transposed matrix. 
	 **/
	Matrix4 Transpose() const;

	/**
	 * Takes the determinant of this matrix.
	 * @return The determinant. 
	 **/
	float Determinant() const;

	/**
	 * Gets the submatrix of this matrix.
	 * @return The submatrix. 
	 **/
	Matrix3 GetSubmatrix(const int32_t &row, const int32_t &col) const;

	/**
	 * Creates a new transformation matrix for a object in 3d space.
	 * @param translation Translation amount the XYZ. 
	 * @param rotation Rotation amount (Pitch, Yaw, Roll). 
	 * @param scale How much to scale the matrix. 
	 * @return The transformation matrix. 
	 **/
	static Matrix4 TransformationMatrix(const Vector3f &translation, const Vector3f &rotation, const Vector3f &scale);

	/**
	 * Creates a new perspective matrix.
	 * @param fov The cameras FOV. 
	 * @param aspectRatio The cameras aspect ratio. 
	 * @param zNear The cameras near plane. 
	 * @param zFar The cameras far plane. 
	 * @return The transformation matrix. 
	 **/
	static Matrix4 PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar);

	/**
	 * Creates a new perspective matrix with a infinite view distance.
	 * @param fov The cameras FOV. 
	 * @param aspectRatio The cameras aspect ratio. 
	 * @param zNear The cameras near plane. 
	 * @return The transformation matrix. 
	 **/
	static Matrix4 PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear);

	/**
	 * Creates a new orthographic matrix.
	 * @param left The left plane. 
	 * @param right The right plane. 
	 * @param bottom The bottom plane. 
	 * @param top The top plane. 
	 * @param zNear The near plane. 
	 * @param zFar The far plane. 
	 * @return The orthographic matrix. 
	 **/
	static Matrix4 OrthographicMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &zNear, const float &zFar);

	/**
	 * Creates a new frustum matrix.
	 * @param left The left plane. 
	 * @param right The right plane. 
	 * @param bottom The bottom plane. 
	 * @param top The top plane. 
	 * @param zNear The near plane. 
	 * @param zFar The far plane. 
	 * @return The frustum matrix. 
	 **/
	static Matrix4 FrustumMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &zNear, const float &zFar);

	/**
	 * Creates a new view matrix.
	 * @param position The cameras position. 
	 * @param rotation The cameras rotation. 
	 * @return The view matrix. 
	 **/
	static Matrix4 ViewMatrix(const Vector3f &position, const Vector3f &rotation);

	/**
	 * Projects a 3D world point into screen space.
	 * @param worldSpace The point in world space.
	 * @param viewMatrix The cameras view matrix. 
	 * @param projectionMatrix The cameras projection matrix. 
	 * @return A 2D point stored in XY, and the distance (Z, if negative the point is behind the screen). 
	 **/
	static Vector3f Project(const Vector3f &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

	/**
	 * Unprojects a screen space point into a 3D world point.
	 * @param screenSpace The point in screen space.
	 * @param viewMatrix The cameras view matrix. 
	 * @param projectionMatrix The cameras projection matrix. 
	 * @return A 2D point stored in XY, and the distance (Z, if negative the point is behind the screen). 
	 **/
	static Vector3f Unproject(const Vector3f &screenSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

	/**
	 * Creates a new matrix that has the camera looking at the target.
	 * @param eye The source position. 
	 * @param centre The target position. 
	 * @param up What view direction is up. 
	 * @return The matrix. 
	 **/
	static Matrix4 LookAt(const Vector3f &eye, const Vector3f &centre, const Vector3f &up = Vector3f::Up);

	std::string ToString() const;

	bool operator==(const Matrix4 &other) const;

	bool operator!=(const Matrix4 &other) const;

	Matrix4 operator-() const;

	const Vector4f &operator[](const uint32_t &index) const;

	Vector4f &operator[](const uint32_t &index);

	ACID_EXPORT friend Matrix4 operator+(const Matrix4 &left, const Matrix4 &right);

	ACID_EXPORT friend Matrix4 operator-(const Matrix4 &left, const Matrix4 &right);

	ACID_EXPORT friend Matrix4 operator*(const Matrix4 &left, const Matrix4 &right);

	ACID_EXPORT friend Matrix4 operator/(const Matrix4 &left, const Matrix4 &right);

	ACID_EXPORT friend Matrix4 operator*(const Vector4f &left, const Matrix4 &right);

	ACID_EXPORT friend Matrix4 operator/(const Vector4f &left, const Matrix4 &right);

	ACID_EXPORT friend Matrix4 operator*(const Matrix4 &left, const Vector4f &right);

	ACID_EXPORT friend Matrix4 operator/(const Matrix4 &left, const Vector4f &right);

	ACID_EXPORT friend Matrix4 operator*(const float &left, const Matrix4 &right);

	ACID_EXPORT friend Matrix4 operator/(const float &left, const Matrix4 &right);

	ACID_EXPORT friend Matrix4 operator*(const Matrix4 &left, const float &right);

	ACID_EXPORT friend Matrix4 operator/(const Matrix4 &left, const float &right);

	Matrix4 &operator+=(const Matrix4 &other);

	Matrix4 &operator-=(const Matrix4 &other);

	Matrix4 &operator*=(const Matrix4 &other);

	Matrix4 &operator/=(const Matrix4 &other);

	Matrix4 &operator*=(const Vector4f &other);

	Matrix4 &operator/=(const Vector4f &other);

	Matrix4 &operator*=(const float &other);

	Matrix4 &operator/=(const float &other);

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, Matrix4 &matrix);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const Matrix4 &matrix);

	ACID_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Matrix4 &matrix);

	static const Matrix4 Identity;
	static const Matrix4 Zero;

	union
	{
		struct
		{
			Vector4f m_rows[4];
		};

		struct
		{
			float m_linear[16];
		};
	};
};
}

namespace std
{
template<>
struct hash<acid::Matrix4>
{
	size_t operator()(acid::Matrix4 const &matrix) const
	{
		size_t seed = 0;
		acid::Maths::HashCombine(seed, matrix[0]);
		acid::Maths::HashCombine(seed, matrix[1]);
		acid::Maths::HashCombine(seed, matrix[2]);
		acid::Maths::HashCombine(seed, matrix[3]);
		return seed;
	}
};
}
