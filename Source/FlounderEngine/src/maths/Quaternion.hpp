#pragma once

#include "Matrix3.hpp"
#include "Matrix4.hpp"

namespace Flounder
{
	/// <summary>
	/// A vector like object of the form w + xi + yj + zk, where w, x, y, z are real numbers and i, j, k are imaginary units.
	/// </summary>
	class Quaternion
	{
	public:
		union
		{
			struct
			{
				float m_x, m_y, m_z, m_w;
			};

			struct
			{
				float m_elements[4];
			};
		};

		/// <summary>
		/// Constructor for quaternion.
		/// </summary>
		Quaternion();

		/// <summary>
		/// Constructor for quaternion.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		Quaternion(const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Constructor for quaternion.
		/// </summary>
		/// <param name="source"> Creates this quaternion out of a existing one. </param>
		Quaternion(const Quaternion &source);

		/// <summary>
		/// Constructor for quaternion.
		/// </summary>
		/// <param name="source"> Creates this quaternion out of a matrix one. </param>
		Quaternion(const Matrix4 &source);

		/// <summary>
		/// Deconstructor for quaternion.
		/// </summary>
		~Quaternion();

		/// <summary>
		/// Sets values in the quaternion.
		/// </summary>
		/// <param name="x"> The new X value. </param>
		/// <param name="y"> The new Y value. </param>
		/// <param name="z"> The new Z value. </param>
		/// <param name="w"> The new W value. </param>
		/// <returns> This. </returns>
		Quaternion *Set(const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Loads from another quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <returns> This. </returns>
		Quaternion *Set(const Quaternion &source);

		/// <summary>
		/// Loads from a equivalent rotation of the axis-angle argument.
		/// </summary>
		/// <param name="source"> The source axis-angle: (x,y,z) is the axis and w is the angle. </param>
		/// <returns> This. </returns>
		Quaternion *Set(const Vector4 &source);

		/// <summary>
		/// Loads from a Matrix4.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <returns> This. </returns>
		Quaternion *Set(const Matrix4 &source);

		/// <summary>
		/// Loads from a Matrix3.
		/// </summary>
		/// <param name="source"> The source matrix. </param>
		/// <returns> This. </returns>
		Quaternion *Set(const Matrix3 &source);

		/// <summary>
		/// Set the given quaternion to the multiplication identity.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <returns> The source quaternion. </returns>
		static Quaternion *SetIdentity(Quaternion *source);

		/// <summary>
		/// Sets the value of this quaternion to the quaternion product of quaternions left and right (this = left * right). Note that this is safe for aliasing (e.g. this can be left or right).
		/// </summary>
		/// <param name="left"> The left source quaternion. </param>
		/// <param name="right"> The right source quaternion. </param>
		/// <param name="destination"> The destination quaternion or nullptr if a new quaternion is to be created.</param>
		/// <returns> The destination quaternion. </returns>
		static Quaternion *Multiply(const Quaternion &left, const Quaternion &right, Quaternion *destination);

		/// <summary>
		/// Multiplies quaternion left by the inverse of quaternion right and places the value into this quaternion. The value of both argument quaternions is persevered (this = left * right^-1).
		/// </summary>
		/// <param name="left"> The left source quaternion. </param>
		/// <param name="right"> The right source quaternion. </param>
		/// <param name="destination"> The destination quaternion or nullptr if a new quaternion is to be created. </param>
		/// <returns> The destination quaternion. </returns>
		static Quaternion *MultiplyInverse(const Quaternion &left, const Quaternion &right, Quaternion *destination);

		/// <summary>
		/// Calculates the dot product of the two quaternions.
		/// </summary>
		/// <param name="left"> The left source quaternion. </param>
		/// <param name="right"> The right source quaternion. </param>
		/// <returns> Left dot right. </returns>
		static float Dot(const Quaternion &left, const Quaternion &right);

		/// <summary>
		/// Calculates the slerp between the two quaternions, they must be normalized!
		/// </summary>
		/// <param name="left"> The left source normalized quaternion. </param>
		/// <param name="right"> The right source normalized quaternion.</param>
		/// <param name="progression"> The progression. </param>
		/// <param name="destination"> The destination quaternion or nullptr if a new quaternion is to be created.
		/// <returns> Left slerp right. </returns>
		static Quaternion *Slerp(const Quaternion &left, const Quaternion &right, const float &progression, Quaternion *destination);

		/// <summary>
		/// Scales a quaternion by a scalar and places the result in the destination quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="scalar"> The scalar value. </param>
		/// <param name="destination"> The destination quaternion or nullptr if a new quaternion is to be created. </param>
		/// <returns> The destination quaternion. </returns>
		static Quaternion *Scale(const Quaternion &source, const float &scalar, Quaternion *destination);

		/// <summary>
		/// Negates a quaternion and places the result in the destination quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="destination"> The destination quaternion or nullptr if a new quaternion is to be created. </param>
		/// <returns> The destination quaternion. </returns>
		static Quaternion *Negate(const Quaternion &source, Quaternion *destination);

		/// <summary>
		/// Normalizes a quaternion and places the result in the destination quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="destination"> The destination quaternion or nullptr if a new quaternion is to be created.</param>
		/// <returns> The destination quaternion. </returns>
		static Quaternion *Normalize(const Quaternion &source, Quaternion *destination);

		/// <summary>
		/// Gets the length of the quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <returns> The length of the quaternion. </returns>
		static float Length(const Quaternion &source);

		/// <summary>
		/// Gets the length of the quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <returns> The length squared of the quaternion. </returns>
		static float LengthSquared(const Quaternion &source);

		/// <summary>
		/// Converts the quaternion to a 4x4 matrix.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="destination"> The destination matrix.</param>
		/// <returns> The rotation matrix which represents the exact same rotation as this quaternion. </returns>
		static Matrix4 *ToMatrix(const Quaternion &source, Matrix4 *destination);

		/// <summary>
		/// Converts the quaternion to a 4x4 matrix representing the exact same
		/// rotation as this quaternion. (The rotation is only contained in the
		/// top-left 3x3 part, but a 4x4 matrix is returned here for convenience
		/// seeing as it will be multiplied with other 4x4 matrices).
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <param name="destination"> The destination matrix.</param>
		/// <returns> The rotation matrix which represents the exact same rotation as this quaternion. </returns>
		static Matrix4 *ToRotationMatrix(const Quaternion &source, Matrix4 *destination);

		/// <summary>
		/// Set this quaternion to the multiplication identity.
		/// </summary>
		/// <returns> This </returns>
		Quaternion *SetIdentity();

		/// <summary>
		/// Negates this quaternion.
		/// </summary>
		/// <returns> This. </returns>
		Quaternion *Negate();

		/// <summary>
		/// Normalizes this quaternion.
		/// </summary>
		/// <returns> This. </returns>
		Quaternion *Normalize();

		/// <summary>
		/// Scales this quaternion.
		/// </summary>
		/// <param name="scalar"> The scale factor. </param>
		/// <returns> This. </returns>
		Quaternion *Scale(const float &scalar);

		/// <summary>
		/// Gets if all the components to the quaternion are zero.
		/// </summary>
		/// <returns> If the quaternion is zero. </returns>
		bool IsZero() const;

		/// <summary>
		/// Gets the length of the quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <returns> The length of the quaternion. </returns>
		float Length() const;

		/// <summary>
		/// Gets the length squared of the quaternion.
		/// </summary>
		/// <param name="source"> The source quaternion. </param>
		/// <returns> The length squared of the quaternion. </returns>
		float LengthSquared() const;
	};
}
