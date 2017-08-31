#pragma once

#include "maths.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace Flounder
{
	/// <summary>
	/// Holds a 4x4 matrix.
	/// </summary>
	class Matrix4
	{
	public:
		float m_00;
		float m_01;
		float m_02;
		float m_03;
		float m_10;
		float m_11;
		float m_12;
		float m_13;
		float m_20;
		float m_21;
		float m_22;
		float m_23;
		float m_30;
		float m_31;
		float m_32;
		float m_33;

		/// <summary>
		/// Constructor for Matrix4. The matrix is initialised to the identity.
		/// </summary>
		Matrix4();

		/// <summary>
		/// Constructor for Matrix4.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a existing one. </param>
		Matrix4(const Matrix4 &source);

		/// <summary>
		/// Constructor for Matrix4.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 16 element array. </param>
		Matrix4(const float source[16]);

		/// <summary>
		/// Deconstructor for Matrix4.
		/// </summary>
		~Matrix4();

		/// <summary>
		/// Loads from another Matrix4.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <returns> This. </returns>
		Matrix4 *set(const Matrix4 &source);

		/// <summary>
		/// Loads from a 16 element array.
		/// </summary>
		/// <param name="source"> The source array. </param>
		/// <returns> This. </returns>
		Matrix4 *set(const float source[16]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity. </param>
		/// <returns> The source matrix. </returns>
		static Matrix4 *setIdentity(Matrix4 *source);

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// <returns> The determinant of the matrix. </returns>
		static float determinant(const Matrix4 &source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix4 *add(const Matrix4 &left, const Matrix4 &right, Matrix4 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix4 *subtract(const Matrix4 &left, const Matrix4 &right, Matrix4 *destination);

		/// <summary>
		/// Multiplies a matrix and a vector together and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *multiply(const Matrix4 &left, const Vector4 &right, Vector4 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix4 *multiply(const Matrix4 &left, const Matrix4 &right, Matrix4 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector4 *transform(const Matrix4 &left, const Vector4 &right, Vector4 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix4 *scale(const Matrix4 &left, const Vector3 &right, Matrix4 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix4 *scale(const Matrix4 &left, const Vector4 &right, Matrix4 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or nullptr if a new one is to be created. </param>
		/// <returns> The inverted matrix, or nullptr if source can't be reverted. </returns>
		static Matrix4 *invert(const Matrix4 &source, Matrix4 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The negated matrix. </returns>
		static Matrix4 *negate(const Matrix4 &source, Matrix4 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transposed matrix. </returns>
		static Matrix4 *transpose(const Matrix4 &source, Matrix4 *destination);

		/// <summary>
		/// Translates a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix4 *translate(const Matrix4 &left, const Vector2 &right, Matrix4 *destination);

		/// <summary>
		/// Translates a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix4 *translate(const Matrix4 &left, const Vector3 &right, Matrix4 *destination);

		/// <summary>
		/// Rotates a matrix around the given axis the specified angle and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="axis"> The vector representing the rotation axis. Must be normalized. </param>
		/// <param name="angle"> the angle, in radians. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The destination matrix. </returns>
		static Matrix4 *rotate(const Matrix4 &source, const Vector3 &axis, const float &angle, Matrix4 *destination);

		/// <summary>
		/// Turns a 4x4 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array. </param>
		/// <returns> A 16 float array. </returns>
		static float *toArray(const Matrix4 &matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0. </param>
		/// <returns> The matrix set to zero. </returns>
		static Matrix4 *setZero(Matrix4 *source);

		/// <summary>
		/// Rotates a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="rotation"> The rotation amount. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *rotate(const Vector3 &source, const Vector3 &rotation, Vector3 *destination);

		/// <summary>
		/// Creates a new transformation matrix for a object in 2d flounder.space.
		/// </summary>
		/// <param name="translation"> Translation amount the XY. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> Returns the transformation matrix. </returns>
		static Matrix4 *transformationMatrix(const Vector2 &translation, const float &scale, Matrix4 *destination);

		/// <summary>
		/// Creates a new transformation matrix for a object in 2d flounder.space.
		/// </summary>
		/// <param name="translation"> Translation amount the XY. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> Returns the transformation matrix. </returns>
		static Matrix4 *transformationMatrix(const Vector2 &translation, const Vector3 &scale, Matrix4 *destination);

		/// <summary>
		/// Creates a new transformation matrix for a object in 3d flounder.space.
		/// </summary>
		/// <param name="translation"> Translation amount the XYZ. </param>
		/// <param name="rotation"> Rotation amount the XYZ. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> Returns the transformation matrix. </returns>
		static Matrix4 *transformationMatrix(const Vector3 &translation, const Vector3 &rotation, const float &scale, Matrix4 *destination);

		/// <summary>
		/// Creates a new transformation matrix for a object in 3d flounder.space.
		/// </summary>
		/// <param name="translation"> Translation amount the XYZ. </param>
		/// <param name="rotation"> Rotation amount the XYZ. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> Returns the transformation matrix. </returns>
		static Matrix4 *transformationMatrix(const Vector3 &translation, const Vector3 &rotation, const Vector3 &scale, Matrix4 *destination);

		/// <summary>
		/// Creates a new perspective matrix, or updates a existing one.
		/// </summary>
		/// <param name="fov"> The cameras FOV. </param>
		/// <param name="aspectRatio"> The cameras aspect ratio. </param>
		/// <param name="zNear"> The cameras near plane. </param>
		/// <param name="zFar"> The cameras far plane. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transformation matrix. </returns>
		static Matrix4 *perspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar, Matrix4 *destination);

		/// <summary>
		/// Creates a new orthographic matrix, or updates a existing one.
		/// </summary>
		/// <param name="left"> The left plane. </param>
		/// <param name="right"> The right plane. </param>
		/// <param name="bottom"> The bottom plane. </param>
		/// <param name="top"> The top plane. </param>
		/// <param name="near"> The near plane. </param>
		/// <param name="far"> The far plane. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transformation matrix. </returns>
		static Matrix4 *orthographicMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &near, const float &far, Matrix4 *destination);

		/// <summary>
		/// Creates a new view matrix, or updates a existing one.
		/// </summary>
		/// <param name="position"> The cameras position. </param>
		/// <param name="rotation"> The cameras rotation. </param>
		/// <returns> The transformation matrix. </returns>
		static Matrix4 *viewMatrix(const Vector3 &position, const Vector3 &rotation, Matrix4 *destination);

		/// <summary>
		/// Transforms a 3D world point into screen space.
		/// </summary>
		/// <param name="worldSpace"> The point to get into screen space. </param>
		/// <param name="viewMatrix"> The cameras view matrix. </param>
		/// <param name="projectionMatrix"> The cameras projection matrix. </param>
		/// <param name="destination"> The vector to write into. </param>
		/// <returns> A 2D point stored in XY, and the distance (Z, if negative the point is behind the screen). </returns>
		static Vector3 *worldToScreenSpace(const Vector3 &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix, Vector3 *destination);

		/// <summary>
		/// Generates a random unit vector from within a cone.
		/// </summary>
		/// <param name="coneDirection"> The cones direction. </param>
		/// <param name="angle"> The cones major angle. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *generateRandomUnitVectorWithinCone(const Vector3 &coneDirection, const float &angle, Vector3 *destination);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix4 *setIdentity();

		/// <summary>
		/// Gets the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant of the matrix. </returns>
		float determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix4 *invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> This. </returns>
		Matrix4 *negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> This. </returns>
		Matrix4 *transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> This. </returns>
		Matrix4 *setZero();
	private:
		static float determinant3x3(const float &t00, const float &t01, const float &t02, const float &t10, const float &t11, const float &t12, const float &t20, const float &t21, const float &t22);
	};
}
