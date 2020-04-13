#pragma once

#include "Vector.hpp"

namespace acid {
class Matrix2;
class Matrix3;

/**
 * @brief Holds a row major 4x4 matrix.
 */
class ACID_EXPORT Matrix4 {
public:
	/**
	 * Constructor for Matrix4. The matrix is initialised to the identity.
	 * @param diagonal The value set to the diagonals.
	 */
	Matrix4(float diagonal = 1.0f);

	/**
	 * Constructor for Matrix4.
	 * @param source Creates this matrix out of a existing one.
	 */
	Matrix4(const Matrix2 &source);

	/**
	 * Constructor for Matrix4.
	 * @param source Creates this matrix out of a existing one.
	 */
	Matrix4(const Matrix3 &source);

	/**
	 * Constructor for Matrix4.
	 * @param source Creates this matrix out of a 16 element array.
	 */
	explicit Matrix4(const float source[16]);

	/**
	 * Constructor for Matrix4.
	 * @param source Creates this matrix out of a 4 vector array.
	 */
	explicit Matrix4(const Vector4f source[4]);

	/**
	 * Adds this matrix to another matrix.
	 * @param other The other matrix.
	 * @return The resultant matrix.
	 */
	Matrix4 Add(const Matrix4 &other) const;

	/**
	 * Subtracts this matrix to another matrix.
	 * @param other The other matrix.
	 * @return The resultant matrix.
	 */
	Matrix4 Subtract(const Matrix4 &other) const;

	/**
	 * Multiplies this matrix by another matrix.
	 * @param other The other matrix.
	 * @return The resultant matrix.
	 */
	Matrix4 Multiply(const Matrix4 &other) const;

	/**
	 * Multiplies this matrix by a vector.
	 * @param other The other vector.
	 * @return The resultant vector.
	 */
	Vector4f Multiply(const Vector4f &other) const;

	/**
	 * Divides this matrix by another matrix.
	 * @param other The other matrix.
	 * @return The resultant matrix.
	 */
	Matrix4 Divide(const Matrix4 &other) const;

	/**
	 * Transforms this matrix by a vector.
	 * @param other The other vector.
	 * @return The resultant vector.
	 */
	Vector4f Transform(const Vector4f &other) const;

	/**
	 * Translates this matrix by a vector.
	 * @param other The vector.
	 * @return The resultant matrix.
	 */
	Matrix4 Translate(const Vector2f &other) const;

	/**
	 * Translates this matrix by a vector.
	 * @param other The vector.
	 * @return The resultant matrix.
	 */
	Matrix4 Translate(const Vector3f &other) const;

	/**
	 * Scales this matrix by a vector.
	 * @param other The other vector.
	 * @return The resultant matrix.
	 */
	Matrix4 Scale(const Vector3f &other) const;

	/**
	 * Scales this matrix by a vector.
	 * @param other The other vector.
	 * @return The resultant matrix.
	 */
	Matrix4 Scale(const Vector4f &other) const;

	/**
	 * Rotates this matrix around the given axis the specified angle.
	 * @param angle The angle, in radians.
	 * @param axis The vector representing the rotation axis.
	 * @return The rotated matrix.
	 */
	Matrix4 Rotate(float angle, const Vector3f &axis) const;

	/**
	 * Inverts this matrix.
	 * @return The inverted matrix.
	 */
	Matrix4 Negate() const;

	/**
	 * Inverses this matrix.
	 * @return The inversed matrix.
	 */
	Matrix4 Inverse() const;

	/**
	 * Transposes this matrix.
	 * @return The transposed matrix.
	 */
	Matrix4 Transpose() const;

	/**
	 * Takes the determinant of this matrix.
	 * @return The determinant.
	 */
	float Determinant() const;

	/**
	 * Gets the submatrix of this matrix.
	 * @return The submatrix.
	 */
	Matrix3 GetSubmatrix(uint32_t row, uint32_t col) const;

	/**
	 * Creates a new transformation matrix for a object in 3d space.
	 * @param translation Translation amount the XYZ.
	 * @param rotation Rotation amount (Pitch, Yaw, Roll).
	 * @param scale How much to scale the matrix.
	 * @return The transformation matrix.
	 */
	static Matrix4 TransformationMatrix(const Vector3f &translation, const Vector3f &rotation, const Vector3f &scale);

	/**
	 * Creates a new perspective matrix.
	 * @param fov The cameras FOV.
	 * @param aspectRatio The cameras aspect ratio.
	 * @param zNear The cameras near plane.
	 * @param zFar The cameras far plane.
	 * @return The transformation matrix.
	 */
	static Matrix4 PerspectiveMatrix(float fov, float aspectRatio, float zNear, float zFar);

	/**
	 * Creates a new perspective matrix with a infinite view distance.
	 * @param fov The cameras FOV.
	 * @param aspectRatio The cameras aspect ratio.
	 * @param zNear The cameras near plane.
	 * @return The transformation matrix.
	 */
	static Matrix4 PerspectiveMatrix(float fov, float aspectRatio, float zNear);

	/**
	 * Creates a new orthographic matrix.
	 * @param left The left plane.
	 * @param right The right plane.
	 * @param bottom The bottom plane.
	 * @param top The top plane.
	 * @param zNear The near plane.
	 * @param zFar The far plane.
	 * @return The orthographic matrix.
	 */
	static Matrix4 OrthographicMatrix(float left, float right, float bottom, float top, float zNear, float zFar);

	/**
	 * Creates a new frustum matrix.
	 * @param left The left plane.
	 * @param right The right plane.
	 * @param bottom The bottom plane.
	 * @param top The top plane.
	 * @param zNear The near plane.
	 * @param zFar The far plane.
	 * @return The frustum matrix.
	 */
	static Matrix4 FrustumMatrix(float left, float right, float bottom, float top, float zNear, float zFar);

	/**
	 * Creates a new view matrix.
	 * @param position The cameras position.
	 * @param rotation The cameras rotation.
	 * @return The view matrix.
	 */
	static Matrix4 ViewMatrix(const Vector3f &position, const Vector3f &rotation);

	/**
	 * Projects a 3D world point into screen space.
	 * @param worldSpace The point in world space.
	 * @param viewMatrix The cameras view matrix.
	 * @param projectionMatrix The cameras projection matrix.
	 * @return A 2D point stored in XY, and the distance (Z, if negative the point is behind the screen).
	 */
	static Vector3f Project(const Vector3f &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

	/**
	 * Unprojects a screen space point into a 3D world point.
	 * @param screenSpace The point in screen space.
	 * @param viewMatrix The cameras view matrix.
	 * @param projectionMatrix The cameras projection matrix.
	 * @return A 2D point stored in XY, and the distance (Z, if negative the point is behind the screen).
	 */
	static Vector3f Unproject(const Vector3f &screenSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

	/**
	 * Creates a new matrix that has the camera looking at the target.
	 * @param eye The source position.
	 * @param centre The target position.
	 * @param up What view direction is up.
	 * @return The matrix.
	 */
	static Matrix4 LookAt(const Vector3f &eye, const Vector3f &centre, const Vector3f &up = Vector3f::Up);

	const Vector4f &operator[](uint32_t index) const;
	Vector4f &operator[](uint32_t index);

	bool operator==(const Matrix4 &other) const;
	bool operator!=(const Matrix4 &other) const;

	Matrix4 operator-() const;

	friend Matrix4 operator+(const Matrix4 &left, const Matrix4 &right);
	friend Matrix4 operator-(const Matrix4 &left, const Matrix4 &right);
	friend Matrix4 operator*(const Matrix4 &left, const Matrix4 &right);
	friend Matrix4 operator/(const Matrix4 &left, const Matrix4 &right);
	friend Matrix4 operator*(const Vector4f &left, const Matrix4 &right);
	friend Matrix4 operator/(const Vector4f &left, const Matrix4 &right);
	friend Matrix4 operator*(const Matrix4 &left, const Vector4f &right);
	friend Matrix4 operator/(const Matrix4 &left, const Vector4f &right);
	friend Matrix4 operator*(float left, const Matrix4 &right);
	friend Matrix4 operator/(float left, const Matrix4 &right);
	friend Matrix4 operator*(const Matrix4 &left, float right);
	friend Matrix4 operator/(const Matrix4 &left, float right);

	Matrix4 &operator+=(const Matrix4 &other);
	Matrix4 &operator-=(const Matrix4 &other);
	Matrix4 &operator*=(const Matrix4 &other);
	Matrix4 &operator/=(const Matrix4 &other);
	Matrix4 &operator*=(const Vector4f &other);
	Matrix4 &operator/=(const Vector4f &other);
	Matrix4 &operator*=(float other);
	Matrix4 &operator/=(float other);

	friend const Node &operator>>(const Node &node, Matrix4 &matrix);
	friend Node &operator<<(Node &node, const Matrix4 &matrix);
	friend std::ostream &operator<<(std::ostream &stream, const Matrix4 &matrix);

	Vector4f rows[4];
};
}

namespace std {
template<>
struct hash<acid::Matrix4> {
	size_t operator()(const acid::Matrix4 &matrix) const noexcept {
		size_t seed = 0;
		acid::Maths::HashCombine(seed, matrix[0]);
		acid::Maths::HashCombine(seed, matrix[1]);
		acid::Maths::HashCombine(seed, matrix[2]);
		acid::Maths::HashCombine(seed, matrix[3]);
		return seed;
	}
};
}
