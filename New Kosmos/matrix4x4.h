#pragma once

#include <iostream>
#include <cmath>

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace flounder {
	/// <summary>
	/// Holds a 4x4 matrix.
	/// </summary>
	class matrix4x4
	{
	public:
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;

		/// <summary>
		/// Constructor for matrix4x4. The matrix is initialised to the identity.
		/// </summary>
		matrix4x4();

		/// <summary>
		/// Constructor for matrix4x4.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a existing one. </param>
		matrix4x4(matrix4x4 *source);

		/// <summary>
		/// Constructor for matrix4x4.
		/// </summary>
		/// <param name="source"> Creates this matrix out of a 16 element array. </param>
		matrix4x4(float source[16]);

		/// <summary>
		/// Deconstructor for matrix4x4.
		/// </summary>
		~matrix4x4();

		/// <summary>
		/// Loads from another matrix4x4.
		/// </summary>
		/// <param name="source"> The source matrix.
		/// </param>
		/// <returns> This. </returns>
		matrix4x4 *set(matrix4x4 *source);

		/// <summary>
		/// Loads from a 16 element array.
		/// </summary>
		/// <param name="source"> The source array.
		/// </param>
		/// <returns> This. </returns>
		matrix4x4 *set(float source[16]);

		/// <summary>
		/// Set the source matrix to be the identity matrix.
		/// </summary>
		/// <param name="source"> The matrix to set to the identity.
		/// </param>
		/// <returns> The source matrix. </returns>
		static matrix4x4 *setIdentity(matrix4x4 *source);

		/// <summary>
		/// Adds two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix4x4 *add(matrix4x4 *left, matrix4x4 *right, matrix4x4 *destination);

		/// <summary>
		/// Subtracts two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix4x4 *subtract(matrix4x4 *left, matrix4x4 *right, matrix4x4 *destination);

		/// <summary>
		/// Multiplies a matrix and a vector together and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *multiply(matrix4x4 *left, vector4 *right, vector4 *destination);

		/// <summary>
		/// Multiplies two matrices together and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix4x4 *multiply(matrix4x4 *left, matrix4x4 *right, matrix4x4 *destination);

		/// <summary>
		/// Transforms a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector4 *transform(matrix4x4 *left, vector4 *right, vector4 *destination);

		/// <summary>
		/// Scales a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix4x4 *scale(matrix4x4 *left, vector4 *right, matrix4x4 *destination);

		/// <summary>
		/// Inverts the source matrix and puts the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix to be inverted. </param>
		/// <param name="destination"> The destination matrix, or null if a new one is to be created.
		/// </param>
		/// <returns> The inverted matrix, or null if source can't be reverted. </returns>
		static matrix4x4 *invert(matrix4x4 *source, matrix4x4 *destination);

		/// <summary>
		/// Negates the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The negated matrix. </returns>
		static matrix4x4 *negate(matrix4x4 *source, matrix4x4 *destination);

		/// <summary>
		/// Transpose the source matrix and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The transposed matrix. </returns>
		static matrix4x4 *transpose(matrix4x4 *source, matrix4x4 *destination);

		/// <summary>
		/// Translates a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix4x4 *translate(matrix4x4 *left, vector2 *right, matrix4x4 *destination);

		/// <summary>
		/// Translates a matrix by a vector and places the result in the destination matrix.
		/// </summary>
		/// <param name="left"> The left source matrix. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix4x4 *translate(matrix4x4 *left, vector3 *right, matrix4x4 *destination);

		/// <summary>
		/// Rotates a matrix around the given axis the specified angle and places the result in the destination matrix.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <param name="axis"> The vector representing the rotation axis. Must be normalized. </param>
		/// <param name="angle"> the angle, in radians. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The destination matrix. </returns>
		static matrix4x4 *rotate(matrix4x4 *source, vector3 *axis, float angle, matrix4x4 *destination);

		/// <summary>
		/// Turns a 4x4 matrix into an array.
		/// </summary>
		/// <param name="matrix"> The matrix to turn into an array.
		/// </param>
		/// <returns> A 16 float array. </returns>
		static float *toArray(matrix4x4 *matrix);

		/// <summary>
		/// Sets the source matrix to 0.
		/// </summary>
		/// <param name="source"> The matrix to be set to 0.
		/// </param>
		/// <returns> The matrix set to zero. </returns>
		static matrix4x4 *setZero(matrix4x4 *source);

		/// <summary>
		/// Rotates a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="rotation"> The rotation amount. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector3 *rotate(vector3 *source, vector3 *rotation, vector3 *destination);

		/// <summary>
		/// Creates a new transformation matrix for a object in 2d flounder.space.
		/// </summary>
		/// <param name="translation"> Translation amount the XY. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> Returns the transformation matrix. </returns>
		static matrix4x4 *transformationMatrix(vector2 *translation, float scale, matrix4x4 *destination);

		/// <summary>
		/// Creates a new transformation matrix for a object in 2d flounder.space.
		/// </summary>
		/// <param name="translation"> Translation amount the XY. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> Returns the transformation matrix. </returns>
		static matrix4x4 *transformationMatrix(vector2 *translation, vector3 *scale, matrix4x4 *destination);

		/// <summary>
		/// Creates a new transformation matrix for a object in 3d flounder.space.
		/// </summary>
		/// <param name="translation"> Translation amount the XYZ. </param>
		/// <param name="rotation"> Rotation amount the XYZ. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> Returns the transformation matrix. </returns>
		static matrix4x4 *transformationMatrix(vector3 *translation, vector3 *rotation, float scale, matrix4x4 *destination);

		/// <summary>
		/// Creates a new transformation matrix for a object in 3d flounder.space.
		/// </summary>
		/// <param name="translation"> Translation amount the XYZ. </param>
		/// <param name="rotation"> Rotation amount the XYZ. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> Returns the transformation matrix. </returns>
		static matrix4x4 *transformationMatrix(vector3 *translation, vector3 *rotation, vector3 *scale, matrix4x4 *destination);

		/// <summary>
		/// Creates a new perspective matrix, or updates a existing one.
		/// </summary>
		/// <param name="fov"> The cameras FOV. </param>
		/// <param name="aspectRatio"> The cameras aspect ratio. </param>
		/// <param name="zNear"> The cameras near plane. </param>
		/// <param name="zFar"> The cameras far plane. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The transformation matrix. </returns>
		static matrix4x4 *perspectiveMatrix(float fov, float aspectRatio, float zNear, float zFar, matrix4x4 *destination);

		/// <summary>
		/// Creates a new orthographic matrix, or updates a existing one.
		/// </summary>
		/// <param name="left"> The left plane. </param>
		/// <param name="right"> The right plane. </param>
		/// <param name="bottom"> The bottom plane. </param>
		/// <param name="top"> The top plane. </param>
		/// <param name="near"> The near plane. </param>
		/// <param name="far"> The far plane. </param>
		/// <param name="destination"> The destination matrix or null if a new matrix is to be created.
		/// </param>
		/// <returns> The transformation matrix. </returns>
		static matrix4x4 *orthographicMatrix(float left, float right, float bottom, float top, float near, float far, matrix4x4 *destination);

		/// <summary>
		/// Transforms a 3D world point into screen space.
		/// </summary>
		/// <param name="worldSpace"> The point to get into screen space. </param>
		/// <param name="viewMatrix"> The cameras view matrix. </param>
		/// <param name="projectionMatrix"> The cameras projection matrix. </param>
		/// <param name="destination"> The vector to write into.
		/// </param>
		/// <returns> A 2D point stored in XY, and the distance (Z, if negative the point is behind the screen). </returns>
		static vector3 *worldToScreenSpace(vector3 *worldSpace, matrix4x4 *viewMatrix, matrix4x4 *projectionMatrix, vector3 *destination);

		/// <summary>
		/// Generates a random unit vector from within a cone.
		/// </summary>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <param name="coneDirection"> The cones direction. </param>
		/// <param name="angle"> The cones major angle.
		/// </param>
		/// <returns> The destination vector. </returns>
		static vector3 *generateRandomUnitVectorWithinCone(vector3 *destination, vector3 *coneDirection, float angle);

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix4x4 *setIdentity();

		/// <returns> The determinant of the matrix. </returns>
		float determinant();

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix4x4 *invert();

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> this. </returns>
		matrix4x4 *negate();

		/// <summary>
		/// Transposes this matrix
		/// </summary>
		/// <returns> this. </returns>
		matrix4x4 *transpose();

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> this. </returns>
		matrix4x4 *setZero();
	private:
		/// <summary>
		/// Calculates the determinant of a 3x3 matrix.
		/// </summary>
		/// <param name="t00"> m00 </param>
		/// <param name="t01"> m01 </param>
		/// <param name="t02"> m02 </param>
		/// <param name="t10"> m10 </param>
		/// <param name="t11"> m11 </param>
		/// <param name="t12"> m12 </param>
		/// <param name="t20"> m20 </param>
		/// <param name="t21"> m21 </param>
		/// <param name="t22"> m22
		/// </param>
		/// <returns> The determinant of the 3x3 matrix. </returns>
		static float determinant3x3(float t00, float t01, float t02, float t10, float t11, float t12, float t20, float t21, float t22);
	};
}
