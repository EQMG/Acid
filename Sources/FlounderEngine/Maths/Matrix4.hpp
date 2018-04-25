#pragma once

#include <ostream>
#include <string>
#include "Vector4.hpp"

namespace fl
{
	class Vector2;
	class Vector3;

	/// <summary>
	/// Holds a 4x4 matrix.
	/// </summary>
	class F_EXPORT Matrix4
	{
	public:
		union
		{
			struct
			{
				Vector4 *m_0;
				Vector4 *m_1;
				Vector4 *m_2;
				Vector4 *m_3;
			};

			struct
			{
				float m_00, m_01, m_02, m_03;
				float m_10, m_11, m_12, m_13;
				float m_20, m_21, m_22, m_23;
				float m_30, m_31, m_32, m_33;
			};

			struct
			{
				float m_elements[4][4];
			};
		};

		static const Matrix4 IDENTITY;
		static const Matrix4 ZERO;

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
		/// Adds this matrix to another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix4 Add(const Matrix4 &other) const;

		/// <summary>
		/// Subtracts this matrix to another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix4 Subtract(const Matrix4 &other) const;

		/// <summary>
		/// Multiplies this matrix by another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix4 Multiply(const Matrix4 &other) const;

		/// <summary>
		/// Multiplies this matrix by a vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector4 Multiply(const Vector4 &other) const;

		/// <summary>
		/// Divides this matrix by another matrix.
		/// </summary>
		/// <param name="other"> The other matrix. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix4 Divide(const Matrix4 &other) const;

		/// <summary>
		/// Transforms this matrix by a vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant vector. </returns>
		Vector4 Transform(const Vector4 &other) const;

		/// <summary>
		/// Translates this matrix by a vector.
		/// </summary>
		/// <param name="other"> The vector. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix4 Translate(const Vector2 &other) const;

		/// <summary>
		/// Translates this matrix by a vector.
		/// </summary>
		/// <param name="other"> The vector. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix4 Translate(const Vector3 &other) const;

		/// <summary>
		/// Scales this matrix by a vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix4 Scale(const Vector3 &other) const;

		/// <summary>
		/// Scales this matrix by a vector.
		/// </summary>
		/// <param name="other"> The other vector. </param>
		/// <returns> The resultant matrix. </returns>
		Matrix4 Scale(const Vector4 &other) const;

		/// <summary>
		/// Rotates this matrix around the given axis the specified angle.
		/// </summary>
		/// <param name="angle"> the angle, in radians. </param>
		/// <param name="axis"> The vector representing the rotation axis. </param>
		/// <returns> The rotated matrix. </returns>
		Matrix4 Rotate(const float &angle, const Vector3 &axis) const;

		/// <summary>
		/// Inverts this matrix.
		/// </summary>
		/// <returns> The inverted matrix. </returns>
		Matrix4 Negate() const;

		/// <summary>
		/// Negates this matrix.
		/// </summary>
		/// <returns> The negated matrix. </returns>
		Matrix4 Invert() const;

		/// <summary>
		/// Transposes this matrix.
		/// </summary>
		/// <returns> The transposed matrix. </returns>
		Matrix4 Transpose() const;

		/// <summary>
		/// Takes the determinant of this matrix.
		/// </summary>
		/// <returns> The determinant. </returns>
		float Determinant() const;

		/// <summary>
		/// Creates a new transformation matrix for a object in 3d space.
		/// </summary>
		/// <param name="translation"> Translation amount the XYZ. </param>
		/// <param name="rotation"> Rotation amount the XYZ. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <returns> Returns the transformation matrix. </returns>
		static Matrix4 TransformationMatrix(const Vector3 &translation, const Vector3 &rotation, const Vector3 &scale);

		/// <summary>
		/// Creates a new transformation matrix for a object in 2d space.
		/// </summary>
		/// <param name="translation"> Translation amount the XY. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <returns> Returns the transformation matrix. </returns>
		static Matrix4 TransformationMatrix(const Vector2 &translation, const float &scale);

		/// <summary>
		/// Creates a new transformation matrix for a object in 2d space.
		/// </summary>
		/// <param name="translation"> Translation amount the XY. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <returns> Returns the transformation matrix. </returns>
		static Matrix4 TransformationMatrix(const Vector2 &translation, const Vector3 &scale);

		/// <summary>
		/// Creates a new transformation matrix for a object in 3d space.
		/// </summary>
		/// <param name="translation"> Translation amount the XYZ. </param>
		/// <param name="rotation"> Rotation amount the XYZ. </param>
		/// <param name="scale"> How much to scale the matrix. </param>
		/// <returns> Returns the transformation matrix. </returns>
		static Matrix4 TransformationMatrix(const Vector3 &translation, const Vector3 &rotation, const float &scale);

		/// <summary>
		/// Creates a new perspective matrix.
		/// </summary>
		/// <param name="fov"> The cameras FOV. </param>
		/// <param name="aspectRatio"> The cameras aspect ratio. </param>
		/// <param name="zNear"> The cameras near plane. </param>
		/// <param name="zFar"> The cameras far plane. </param>
		/// <returns> The transformation matrix. </returns>
		static Matrix4 PerspectiveMatrix(const float &fov, const float &aspectRatio, const float &zNear, const float &zFar);

		/// <summary>
		/// Creates a new orthographic matrix.
		/// </summary>
		/// <param name="left"> The left plane. </param>
		/// <param name="right"> The right plane. </param>
		/// <param name="bottom"> The bottom plane. </param>
		/// <param name="top"> The top plane. </param>
		/// <param name="near"> The near plane. </param>
		/// <param name="far"> The far plane. </param>
		/// <param name="destination"> The destination matrix or nullptr if a new matrix is to be created. </param>
		/// <returns> The transformation matrix. </returns>
		static Matrix4 OrthographicMatrix(const float &left, const float &right, const float &bottom, const float &top, const float &near, const float &far);

		/// <summary>
		/// Creates a new view matrix.
		/// </summary>
		/// <param name="position"> The cameras position. </param>
		/// <param name="rotation"> The cameras rotation. </param>
		/// <returns> The transformation matrix. </returns>
		static Matrix4 ViewMatrix(const Vector3 &position, const Vector3 &rotation);

		/// <summary>
		/// Transforms a 3D world point into screen space.
		/// </summary>
		/// <param name="worldSpace"> The point to get into screen space. </param>
		/// <param name="viewMatrix"> The cameras view matrix. </param>
		/// <param name="projectionMatrix"> The cameras projection matrix. </param>
		/// <returns> A 2D point stored in XY, and the distance (Z, if negative the point is behind the screen). </returns>
		static Vector3 WorldToScreenSpace(const Vector3 &worldSpace, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix);

		/// <summary>
		/// Sets this matrix to 0.
		/// </summary>
		/// <returns> The identity matrix. </returns>
		Matrix4 SetZero();

		/// <summary>
		/// Sets this matrix to be the identity matrix.
		/// </summary>
		/// <returns> The identity matrix. </returns>
		Matrix4 SetIdentity();

		/// <summary>
		/// Turns this 4x4 matrix into an array.
		/// </summary>
		/// <returns> A 16 float array. </returns>
		float *ToArray() const;

		/// <summary>
		/// Saves this matrix into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		Matrix4 &operator=(const Matrix4 &other);

		Matrix4 &operator=(const float array[16]);

		Matrix4 &operator=(LoadedValue *value);

		bool operator==(const Matrix4 &other) const;

		bool operator!=(const Matrix4 &other) const;

		Matrix4 operator-();

		F_EXPORT friend Matrix4 operator+(Matrix4 left, const Matrix4 &right);

		F_EXPORT friend Matrix4 operator-(Matrix4 left, const Matrix4 &right);

		F_EXPORT friend Matrix4 operator*(Matrix4 left, const Matrix4 &right);

		F_EXPORT friend Matrix4 operator/(Matrix4 left, const Matrix4 &right);

		F_EXPORT friend Matrix4 operator*(Matrix4 left, Vector4 value);

		F_EXPORT friend Matrix4 operator/(Matrix4 left, Vector4 value);

		F_EXPORT friend Matrix4 operator*(Matrix4 left, float value);

		F_EXPORT friend Matrix4 operator/(Matrix4 left, float value);

		F_EXPORT friend Matrix4 operator*(float value, Matrix4 left);

		F_EXPORT friend Matrix4 operator/(float value, Matrix4 left);

		Matrix4 &operator+=(const Matrix4 &other);

		Matrix4 &operator-=(const Matrix4 &other);

		Matrix4 &operator*=(const Matrix4 &other);

		Matrix4 &operator/=(const Matrix4 &other);

		F_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Matrix4 &matrix);

		std::string ToString() const;

	private:
		static float Determinant3x3(const float &t00, const float &t01, const float &t02, const float &t10, const float &t11, const float &t12, const float &t20, const float &t21, const float &t22);
	};
}
