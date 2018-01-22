#pragma once

#include <ostream>
#include <string>

namespace Flounder
{
	class Colour;

	class Vector2;

	class Vector4;

	/// <summary>
	/// Holds a 3-tuple vector.
	/// </summary>
	class Vector3
	{
	public:
		union
		{
			struct
			{
				float m_x, m_y, m_z;
			};

			struct
			{
				float m_elements[3];
			};
		};

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		Vector3();

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector3(const Vector2 &source);

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector3(const Vector3 &source);

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		Vector3(const Vector4 &source);

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing colour. </param>
		Vector3(const Colour &source);

		/// <summary>
		/// Constructor for Vector3.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		Vector3(const float &x, const float &y, const float &z);

		/// <summary>
		/// Deconstructor for Vector3.
		/// </summary>
		~Vector3();

		/// <summary>
		/// Loads from another Vector3.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Vector3 *Set(const Vector2 &source);

		/// <summary>
		/// Loads from another Vector3.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Vector3 *Set(const Vector3 &source);

		/// <summary>
		/// Loads from another Vector4.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		Vector3 *Set(const Vector4 &source);

		/// <summary>
		/// Sets values in the vector.
		/// </summary>
		/// <param name="x"> The new X value. </param>
		/// <param name="y"> The new Y value. </param>
		/// <param name="z"> The new Z value. </param>
		/// <returns> This. </returns>
		Vector3 *Set(const float &x, const float &y, const float &z);

		/// <summary>
		/// Adds two vectors together and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Add(const Vector3 &left, const Vector3 &right, Vector3 *destination);

		/// <summary>
		/// Subtracts two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Subtract(const Vector3 &left, const Vector3 &right, Vector3 *destination);

		/// <summary>
		/// Multiplies two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Multiply(const Vector3 &left, const Vector3 &right, Vector3 *destination);

		/// <summary>
		/// Divides two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Divide(const Vector3 &left, const Vector3 &right, Vector3 *destination);

		/// <summary>
		/// Calculates the angle between two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <returns> The angle between the two vectors, in radians. </returns>
		static float Angle(const Vector3 &left, const Vector3 &right);

		/// <summary>
		/// Calculates the dot product of the two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <returns> Left dot right. </returns>
		static float Dot(const Vector3 &left, const Vector3 &right);

		/// <summary>
		/// Takes the cross product of two vectors and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Cross(const Vector3 &left, const Vector3 &right, Vector3 *destination);

		/// <summary>
		/// Scales a vector by a scalar and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="scalar"> The scalar value. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Scale(const Vector3 &source, const float &scalar, Vector3 *destination);

		/// <summary>
		/// Instead of calling Vector3::rotate, call Matrix4::rotate! This method will throw a exception!
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="rotation"> The rotation amount. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Rotate(const Vector3 &source, const Vector3 &rotation, Vector3 *destination);

		/// <summary>
		/// Negates a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Negate(const Vector3 &source, Vector3 *destination);

		/// <summary>
		/// Normalizes a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *Normalize(const Vector3 &source, Vector3 *destination);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length of the vector. </returns>
		static float Length(const Vector3 &source);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length squared of the vector. </returns>
		static float LengthSquared(const Vector3 &source);

		/// <summary>
		/// Gets the maximum vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The maximum vector. </returns>
		static Vector3 *MaxVector(const Vector3 &a, const Vector3 &b, Vector3 *destination);

		/// <summary>
		/// Gets the lowest vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The lowest vector. </returns>
		static Vector3 *MinVector(const Vector3 &a, const Vector3 &b, Vector3 *destination);

		/// <summary>
		/// Gets the maximum value in a vector.
		/// </summary>
		/// <param name="vector"> The value to get the maximum value from. </param>
		/// <returns> The maximum value. </returns>
		static float MaxComponent(const Vector3 &vector);

		/// <summary>
		/// Gets the lowest value in a vector.
		/// </summary>
		/// <param name="vector"> The value to get the lowest value from. </param>
		/// <returns> The lowest value. </returns>
		static float MinComponent(const Vector3 &vector);

		/// <summary>
		/// Gets the distance between two points squared.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <returns> The squared distance between the two points. </returns>
		static float GetDistanceSquared(const Vector3 &point1, const Vector3 &point2);

		/// <summary>
		/// Gets the total distance between 2 vectors.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <returns> The total distance between the points. </returns>
		static float GetDistance(const Vector3 &point1, const Vector3 &point2);

		/// <summary>
		/// Gets the vector distance between 2 vectors.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The vector distance between the points. </returns>
		static Vector3 *GetVectorDistance(const Vector3 &point1, const Vector3 &point2, Vector3 *destination);

		/// <summary>
		/// Generates a random unit vector.
		/// </summary>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *RandomUnitVector(Vector3 *destination);

		/// <summary>
		/// Gets a random point from on a circle.
		/// </summary>
		/// <param name="normal"> The circles normal. </param>
		/// <param name="radius"> The circles radius. </param>
		/// <param name="destination"> The destination vector or nullptr if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static Vector3 *RandomPointOnCircle(const Vector3 &normal, const float &radius, Vector3 *destination);

		/// <summary>
		/// Gets the height on a point off of a 3d triangle.
		/// </summary>
		/// <param name="p1"> Point 1 on the triangle. </param>
		/// <param name="p2"> Point 2 on the triangle. </param>
		/// <param name="p3"> Point 3 on the triangle. </param>
		/// <param name="pos"> The XZ position of the object. </param>
		/// <returns> Height of the triangle at the position. </returns>
		static float BaryCentric(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &pos);

		/// <summary>
		/// Translates this vector.
		/// </summary>
		/// <param name="x"> The translation in x. </param>
		/// <param name="y"> the translation in y. </param>
		/// <param name="z"> the translation in z. </param>
		/// <returns> This. </returns>
		Vector3 *Translate(const float &x, const float &y, const float &z);

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns> This. </returns>
		Vector3 *Negate();

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> This. </returns>
		Vector3 *Normalize();

		/// <summary>
		/// Scales this vector.
		/// </summary>
		/// <param name="scalar"> The scale factor. </param>
		/// <returns> This. </returns>
		Vector3 *Scale(const float &scalar);

		/// <summary>
		/// Gets if all the components to the vector are zero.
		/// </summary>
		/// <returns> If the vector is zero. </returns>
		bool IsZero() const;

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length of the vector. </returns>
		float Length() const;

		/// <summary>
		/// Gets the length squared of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length squared of the vector. </returns>
		float LengthSquared() const;

		bool operator==(const Vector3 &other) const;

		bool operator!=(const Vector3 &other) const;

		bool operator<(const Vector3 &other) const;

		bool operator<=(const Vector3 &other) const;

		bool operator>(const Vector3 &other) const;

		bool operator>=(const Vector3 &other) const;

		Vector3 &operator-();

		friend Vector3 operator+(Vector3 left, const Vector3 &right);

		friend Vector3 operator-(Vector3 left, const Vector3 &right);

		friend Vector3 operator*(Vector3 left, const Vector3 &right);

		friend Vector3 operator/(Vector3 left, const Vector3 &right);

		friend Vector3 operator+(Vector3 left, float value);

		friend Vector3 operator-(Vector3 left, float value);

		friend Vector3 operator*(Vector3 left, float value);

		friend Vector3 operator/(Vector3 left, float value);

		friend Vector3 operator+(float value, Vector3 left);

		friend Vector3 operator-(float value, Vector3 left);

		friend Vector3 operator*(float value, Vector3 left);

		friend Vector3 operator/(float value, Vector3 left);

		Vector3 &operator+=(const Vector3 &other);

		Vector3 &operator-=(const Vector3 &other);

		Vector3 &operator*=(const Vector3 &other);

		Vector3 &operator/=(const Vector3 &other);

		Vector3 &operator+=(float value);

		Vector3 &operator-=(float value);

		Vector3 &operator*=(float value);

		Vector3 &operator/=(float value);

		friend std::ostream &operator<<(std::ostream &stream, const Vector3 &vector);

		std::string ToString() const;
	};
}
