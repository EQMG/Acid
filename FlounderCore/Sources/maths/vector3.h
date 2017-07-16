#pragma once

#include "maths.h"

#include "vector2.h"
#include "vector4.h"

namespace flounder {
	/// <summary>
	/// Holds a 3-tuple vector.
	/// </summary>
	class vector3
	{
	public:
		float x, y, z;

		/// <summary>
		/// Constructor for vector3.
		/// </summary>
		vector3();

		/// <summary>
		/// Constructor for vector3.
		/// </summary>
		/// <param name="x"> Start x. </param>
		/// <param name="y"> Start y. </param>
		/// <param name="z"> Start z. </param>
		vector3(const float &x, const float &y, const float &z);

		/// <summary>
		/// Constructor for vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		vector3(const vector3 &source);

		/// <summary>
		/// Constructor for vector3.
		/// </summary>
		/// <param name="source"> Creates this vector out of a existing one. </param>
		vector3(const vector4 &source);

		/// <summary>
		/// Deconstructor for vector3.
		/// </summary>
		~vector3();

		/// <summary>
		/// Sets values in the vector.
		/// </summary>
		/// <param name="x"> The new X value. </param>
		/// <param name="y"> The new Y value. </param>
		/// <param name="z"> The new Z value. </param>
		/// <returns> This. </returns>
		vector3 *set(const float &x, const float &y, const float &z);

		/// <summary>
		/// Loads from another vector3.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		vector3 *set(const vector3 &source);

		/// <summary>
		/// Loads from another vector4.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> This. </returns>
		vector3 *set(const vector4 &source);

		/// <summary>
		/// Adds two vectors together and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *add(const vector3 &left, const vector3 &right, vector3 *destination);

		/// <summary>
		/// Subtracts two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *subtract(const vector3 &left, const vector3 &right, vector3 *destination);

		/// <summary>
		/// Multiplies two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *multiply(const vector3 &left, const vector3 &right, vector3 *destination);

		/// <summary>
		/// Divides two vectors from each other and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *divide(const vector3 &left, const vector3 &right, vector3 *destination);

		/// <summary>
		/// Calculates the angle between two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <returns> The angle between the two vectors, in radians. </returns>
		static float angle(const vector3 &left, const vector3 &right);

		/// <summary>
		/// Calculates the dot product of the two vectors.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <returns> Left dot right. </returns>
		static float dot(const vector3 &left, const vector3 &right);

		/// <summary>
		/// Takes the cross product of two vectors and places the result in the destination vector.
		/// </summary>
		/// <param name="left"> The left source vector. </param>
		/// <param name="right"> The right source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *cross(const vector3 &left, const vector3 &right, vector3 *destination);

		/// <summary>
		/// Scales a vector by a scalar and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="scalar"> The scalar value. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *scale(const vector3 &source, const float &scalar, vector3 *destination);

		/// <summary>
		/// Instead of calling vector3::rotate, call matrix4x4::rotate! This method will throw a exception!
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="rotation"> The rotation amount. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *rotate(const vector3 &source, vector3 *rotation, vector3 *destination);

		/// <summary>
		/// Negates a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *negate(const vector3 &source, vector3 *destination);

		/// <summary>
		/// Normalizes a vector and places the result in the destination vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *normalize(const vector3 &source, vector3 *destination);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length of the vector. </returns>
		static float length(const vector3 &source);

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length squared of the vector. </returns>
		static float lengthSquared(const vector3 &source);

		/// <summary>
		/// Gets the maximum vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The maximum vector. </returns>
		static vector3 *maxVector(const vector3 &a, const vector3 &b, vector3 *destination);

		/// <summary>
		/// Gets the lowest vector size.
		/// </summary>
		/// <param name="a"> The first vector to get values from. </param>
		/// <param name="b"> The second vector to get values from. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The lowest vector. </returns>
		static vector3 *minVector(const vector3 &a, const vector3 &b, vector3 *destination);

		/// <summary>
		/// Gets the maximum value in a vector.
		/// </summary>
		/// <param name="vector"> The value to get the maximum value from. </param>
		/// <returns> The maximum value. </returns>
		static float maxComponent(const vector3 &vector);

		/// <summary>
		/// Gets the lowest value in a vector.
		/// </summary>
		/// <param name="vector"> The value to get the lowest value from. </param>
		/// <returns> The lowest value. </returns>
		static float minComponent(const vector3 &vector);

		/// <summary>
		/// Gets the distance between two points squared.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <returns> The squared distance between the two points. </returns>
		static float getDistanceSquared(const vector3 &point1, const vector3 &point2);

		/// <summary>
		/// Gets the total distance between 2 vectors.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <returns> The total distance between the points. </returns>
		static float getDistance(const vector3 &point1, const vector3 &point2);

		/// <summary>
		/// Gets the vector distance between 2 vectors.
		/// </summary>
		/// <param name="point1"> The first point. </param>
		/// <param name="point2"> The second point. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The vector distance between the points. </returns>
		static vector3 *getVectorDistance(const vector3 &point1, const vector3 &point2, vector3 *destination);

		/// <summary>
		/// Generates a random unit vector.
		/// </summary>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *generateRandomUnitVector(vector3 *destination);

		/// <summary>
		/// Gets a random point from on a circle.
		/// </summary>
		/// <param name="normal"> The circles normal. </param>
		/// <param name="radius"> The circles radius. </param>
		/// <param name="destination"> The destination vector or null if a new vector is to be created. </param>
		/// <returns> The destination vector. </returns>
		static vector3 *randomPointOnCircle(const vector3 &normal, const float &radius, vector3 *destination);

		/// <summary>
		/// Gets the height on a point off of a 3d triangle.
		/// </summary>
		/// <param name="p1"> Point 1 on the triangle. </param>
		/// <param name="p2"> Point 2 on the triangle. </param>
		/// <param name="p3"> Point 3 on the triangle. </param>
		/// <param name="pos"> The XZ position of the object. </param>
		/// <returns> Height of the triangle at the position. </returns>
		static float baryCentric(const vector3 &p1, const vector3 &p2, const vector3 &p3, const vector3 &pos);

		/// <summary>
		/// Translates this vector.
		/// </summary>
		/// <param name="x"> The translation in x. </param>
		/// <param name="y"> the translation in y. </param>
		/// <param name="z"> the translation in z. </param>
		/// <returns> This. </returns>
		vector3 *translate(const float &x, const float &y, const float &z);

		/// <summary>
		/// Negates this vector.
		/// </summary>
		/// <returns> This. </returns>
		vector3 *negate();

		/// <summary>
		/// Normalizes this vector.
		/// </summary>
		/// <returns> This. </returns>
		vector3 *normalize();

		/// <summary>
		/// Scales this vector.
		/// </summary>
		/// <param name="scalar"> The scale factor. </param>
		/// <returns> This. </returns>
		vector3 *scale(const float &scalar);

		/// <summary>
		/// Gets if all the components to the vector are zero.
		/// </summary>
		/// <returns> If the vector is zero. </returns>
		bool isZero();

		/// <summary>
		/// Gets the length of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length of the vector. </returns>
		float length();

		/// <summary>
		/// Gets the length squared of the vector.
		/// </summary>
		/// <param name="source"> The source vector. </param>
		/// <returns> The length squared of the vector. </returns>
		float lengthSquared();
	};
}
