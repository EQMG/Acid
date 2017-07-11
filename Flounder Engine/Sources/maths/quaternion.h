#pragma once

#include "maths.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

namespace flounder {
	/// <summary>
	/// A vector like object of the form w + xi + yj + zk, where w, x, y, z are real numbers and i, j, k are imaginary units.
	/// </summary>
	class quaternion
	{
	public:
		float x, y, z, w;

		/// <summary>
		/// Constructor for quaternion.
		/// </summary>
		quaternion();

		/// <summary>
		/// Constructor for quaternion.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		quaternion(const float x, const float y, const float z, const float w);

		/// <summary>
		/// Constructor for quaternion.
		/// </summary>
		/// <param name="source"> Creates this quaternion out of a existing one. </param>
		quaternion(quaternion *source);

		/// <summary>
		/// Constructor for quaternion.
		/// </summary>
		/// <param name="source"> Creates this quaternion out of a matrix one. </param>
		quaternion(matrix4x4 *source);

		/// <summary>
		/// Deconstructor for quaternion.
		/// </summary>
		~quaternion();

		/// <summary>
		/// Sets values in the quaternion.
		/// </summary>
		/// <param name="x"> The new X value. </param>
		/// <param name="y"> The new Y value. </param>
		/// <param name="z"> The new Z value. </param>
		/// <param name="w"> The new W value.
		/// </param>
		/// <returns> This. </returns>
		quaternion *set(const float x, const float y, const float z, const float w);

		/// <summary>
		/// Loads from another quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion.
		/// </param>
		/// <returns> This. </returns>
		quaternion *set(quaternion *source);

		/// <summary>
		/// Loads from a equivalent rotation of the axis-angle argument.
		/// </summary>
		/// <param name="source"> The source axis-angle: (x,y,z) is the axis and w is the angle. </param>
		/// <returns> This. </returns>
		quaternion *set(vector4 *source);

		/// <summary>
		/// Loads from a matrix4x4.
		/// </summary>
		/// <param name="source"> The source matrix.
		/// </param>
		/// <returns> This. </returns>
		quaternion *set(matrix4x4 *source);

		/// <summary>
		/// Loads from a matrix3x3.
		/// </summary>
		/// <param name="source"> The source matrix.
		/// </param>
		/// <returns> This. </returns>
		quaternion *set(matrix3x3 *source);

		/// <summary>
		/// Set the given quaternion to the multiplication identity.
		/// </summary>
		/// <param name="q"> The quaternion
		/// </param>
		/// <returns> q </returns>
		static quaternion *setIdentity(quaternion *source);

		/// <summary>
		/// Sets the value of this quaternion to the quaternion product of quaternions left and right (this = left * right). Note that this is safe for aliasing (e.g. this can be left or right).
		/// </summary>
		/// <param name="left"> The left source quaternion. </param>
		/// <param name="right"> The right source quaternion. </param>
		/// <param name="destination"> The destination quaternion or null if a new quaternion is to be created.
		/// </param>
		/// <returns> The destination quaternion. </returns>
		static quaternion *multiply(quaternion *left, quaternion *right, quaternion *destination);

		/// <summary>
		/// Multiplies quaternion left by the inverse of quaternion right and places the value into this quaternion. The value of both argument quaternions is persevered (this = left * right^-1).
		/// </summary>
		/// <param name="left"> The left source quaternion. </param>
		/// <param name="right"> The right source quaternion. </param>
		/// <param name="destination"> The destination quaternion or null if a new quaternion is to be created.
		/// </param>
		/// <returns> The destination quaternion. </returns>
		static quaternion *multiplyInverse(quaternion *left, quaternion *right, quaternion *destination);

		/// <summary>
		/// Calculates the dot product of the two quaternions.
		/// </summary>
		/// <param name="left"> The left source quaternion. </param>
		/// <param name="right"> The right source quaternion.
		/// </param>
		/// <returns> Left dot right. </returns>
		static float dot(quaternion *left, quaternion *right);

		/// <summary>
		/// Calculates the slerp between the two quaternions.
		/// </summary>
		/// <param name="left"> The left source quaternion. </param>
		/// <param name="right"> The right source quaternion.</param>
		/// <param name="progression"> The progression. </param>
		/// <returns> Left slerp right. </returns>
		static quaternion *slerp(quaternion *left, quaternion *right, const float progression);

		/// <summary>
		/// Scales a quaternion by a scalar and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="scalar"> The scalar value. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created.
		/// </param>
		/// <returns> The destination vector. </returns>
		static quaternion *scale(quaternion *source, const float scalar, quaternion *destination);

		/// <summary>
		/// Negates a quaternion and places the result in the destination quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="destination"> The destination quaternion or null if a new quaternion is to be created.
		/// </param>
		/// <returns> The destination quaternion. </returns>
		static quaternion *negate(quaternion *source, quaternion *destination);

		/// <summary>
		/// Normalizes a quaternion and places the result in the destination quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="destination"> The destination quaternion or null if a new quaternion is to be created.</param>
		/// <returns> The destination quaternion. </returns>
		static quaternion *normalize(quaternion *source, quaternion *destination);

		/// <summary>
		/// Converts the quaternion to a 4x4 matrix.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="destination"> The destination matrix.</param>
		/// <returns> The rotation matrix which represents the exact same rotation as this quaternion. </returns>
		static matrix4x4 *toMatrix(quaternion *source, matrix4x4 *destination);

		/// <summary>
		/// Converts the quaternion to a 4x4 matrix representing the exact same
		/// rotation as this quaternion. (The rotation is only contained in the
		/// top-left 3x3 part, but a 4x4 matrix is returned here for convenience
		/// seeing as it will be multiplied with other 4x4 matrices).
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="destination"> The destination matrix.</param>
		/// <returns> The rotation matrix which represents the exact same rotation as this quaternion. </returns>
		static matrix4x4 *toRotationMatrix(quaternion *source, matrix4x4 *destination);

		/// <summary>
		/// Set this quaternion to the multiplication identity.
		/// </summary>
		/// <returns> This </returns>
		quaternion *setIdentity();

		/// <summary>
		/// Negates this quaternion.
		/// </summary>
		/// <returns> This. </returns>
		quaternion *negate();

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> This. </returns>
		quaternion *normalize();

		/// <summary>
		/// Scales this quaternion.
		/// </summary>
		/// <param name="scalar"> The scale factor.
		/// </param>
		/// <returns> This. </returns>
		quaternion *scale(const float scalar);

		/// <summary>
		/// Gets if all the components to the quaternion are zero.
		/// </summary>
		/// <returns> If the quaternion is zero. </returns>
		bool isZero();

		/// <returns> The length of the quaternion. </returns>
		float length();

		/// <returns> The length squared of the quaternion. </returns>
		float lengthSquared();
	};
}
