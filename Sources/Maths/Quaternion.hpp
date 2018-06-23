#pragma once

#include <ostream>
#include <string>
#include "Matrix4.hpp"
#include "Vector4.hpp"

namespace fl
{
	/// <summary>
	/// A vector like object of the form w + xi + yj + zk, where w, x, y, z are real numbers and i, j, k are imaginary units.
	/// </summary>
	class FL_EXPORT Quaternion
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

		static const Quaternion ZERO;
		static const Quaternion ONE;
		static const Quaternion POSITIVE_INFINITY;
		static const Quaternion NEGATIVE_INFINITY;

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		Quaternion();

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		/// <param name="w"> Start w. </param>
		Quaternion(const float &x, const float &y, const float &z, const float &w);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="source"> Creates this quaternion out of a existing vector. </param>>
		Quaternion(const Vector4 &source);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Quaternion(const Quaternion &source);

		/// <summary>
		/// Constructor for Quaternion.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing matrix. </param>
		Quaternion(const Matrix4 &source);

		/// <summary>
		/// Deconstructor for Quaternion.
		/// </summary>
		~Quaternion();

		/// <summary>
		/// Sets the value of this quaternion to the quaternion product of quaternions left and right (this = left * right). Note that this is safe for aliasing (e.g. this can be left or right).
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <returns> The resultant quaternion. </returns>
		Quaternion Multiply(const Quaternion &other) const;

		/// <summary>
		/// Multiplies quaternion left by the inverse of quaternion right and places the value into this quaternion. The value of both argument quaternions is persevered (this = left * right^-1).
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <returns> The resultant quaternion. </returns>
		Quaternion MultiplyInverse(const Quaternion &other) const;

		/// <summary>
		/// Calculates the dot product of the this quaternion and another quaternion.
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <returns> The dot product. </returns>
		float Dot(const Quaternion &other) const;

		/// <summary>
		/// Calculates the slerp between this quaternion and another quaternion, they must be normalized!
		/// </summary>
		/// <param name="other"> The other quaternion. </param>
		/// <param name="progression"> The progression. </param>
		/// <returns> Left slerp right. </returns>
		Quaternion Slerp(const Quaternion &other, const float &progression);

		/// <summary>
		/// Scales this quaternion by a scalar.
		/// </summary>
		/// <param name="scalar"> The scalar value. </param>
		/// <returns> The scaled quaternion. </returns>
		Quaternion Scale(const float &scalar) const;

		/// <summary>
		/// Negates this quaternion.
		/// </summary>
		/// <returns> The negated quaternion. </returns>
		Quaternion Negate() const;

		/// <summary>
		/// Normalizes this quaternion.
		/// </summary>
		/// <returns> The normalized quaternion. </returns>
		Quaternion Normalize() const;

		/// <summary>
		/// Gets the length squared of this quaternion.
		/// </summary>
		/// <returns> The length squared. </returns>
		float LengthSquared() const;

		/// <summary>
		/// Gets the length of this quaternion.
		/// </summary>
		/// <returns> The length. </returns>
		float Length() const;

		/// <summary>
		/// Gets the maximum value in this quaternion.
		/// </summary>
		/// <returns> The largest components. </returns>
		float MaxComponent() const;

		/// <summary>
		/// Gets the lowest value in this quaternion.
		/// </summary>
		/// <returns> The smallest components. </returns>
		float MinComponent() const;

		/// <summary>
		/// Converts this quaternion to a 4x4 matrix.
		/// </summary>
		/// <returns> The rotation matrix which represents the exact same rotation as this quaternion. </returns>
		Matrix4 ToMatrix();

		/// <summary>
		/// Converts this quaternion to a 4x4 matrix representing the exact same
		/// rotation as this quaternion. (The rotation is only contained in the
		/// top-left 3x3 part, but a 4x4 matrix is returned here for convenience
		/// seeing as it will be multiplied with other 4x4 matrices).
		/// </summary>
		/// <returns> The rotation matrix which represents the exact same rotation as this quaternion. </returns>
		Matrix4 ToRotationMatrix();

		/// <summary>
		/// Saves this quaternion into a loaded value.
		/// </summary>
		/// <param name="destination"> The destination loaded value. </param>
		void Write(LoadedValue *destination);

		float GetX() const { return m_x; }

		void SetX(const float &x) { m_x = x; }

		float GetY() const { return m_y; }

		void SetY(const float &y) { m_y = y; }

		float GetZ() const { return m_z; }

		void SetZ(const float &z) { m_z = z; }

		float GetW() const { return m_w; }

		void SetW(const float &w) { m_w = w; }

		Quaternion &operator=(const Quaternion &other);

		Quaternion &operator=(const Matrix4 &other);

		Quaternion &operator=(LoadedValue *value);

		bool operator==(const Quaternion &other) const;

		bool operator!=(const Quaternion &other) const;

		bool operator<(const Quaternion &other) const;

		bool operator<=(const Quaternion &other) const;

		bool operator>(const Quaternion &other) const;

		bool operator>=(const Quaternion &other) const;

		bool operator==(const float &value) const;

		bool operator!=(const float &value) const;

		Quaternion operator-();

		FL_EXPORT friend Quaternion operator*(Quaternion left, const Quaternion &right);

		FL_EXPORT friend Quaternion operator*(float value, Quaternion left);

		Quaternion &operator*=(const Quaternion &other);

		Quaternion &operator*=(float value);

		FL_EXPORT friend std::ostream &operator<<(std::ostream &stream, const Quaternion &vector);

		std::string ToString() const;
	};
}
