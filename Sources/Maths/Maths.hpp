#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief Class that holds many various math functions.
 **/
class ACID_EXPORT Maths
{
public:
	static constexpr float Pi = 3.14159265358979323846264338f;
	static constexpr float DegToRad = Pi / 180.0f;
	static constexpr float RadToDeg = 180.0f / Pi;

	/**
	 * Generates a random value from between a range.
	 * @param min The min value. 
	 * @param max The max value. 
	 * @return The randomly selected value within the range. 
	 **/
	static float Random(const float &min = 0.0f, const float &max = 1.0f);

	/**
	 * Generates a single value from a normal distribution, using Box-Muller.
	 * https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
	 * @param standardDeviation The standards deviation of the distribution. 
	 * @param mean The mean of the distribution. 
	 * @return A normally distributed value. 
	 **/
	static float RandomNormal(const float &standardDeviation, const float &mean);

	/**
	 * Creates a number between two numbers, logarithmic.
	 * @param min The min value. 
	 * @param max The max value. 
	 * @return The final random number. 
	 **/
	static float RandomLog(const float &min, const float &max);

	/**
	 * Converts degrees to radians.
	 * @param degrees The degrees value. 
	 * @return The radians value. 
	 **/
	static float Radians(const float &degrees);

	/**
	 * Converts radians to degrees.
	 * @param radians The radians value. 
	 * @return The degrees value. 
	 **/
	static float Degrees(const float &radians);

	/**
	 * Normalizes a angle into the range of 0-360.
	 * @param degrees The source angle. 
	 * @return The normalized angle. 
	 **/
	static float WrapDegrees(const float &degrees);

	/**
	 * Normalizes a angle into the range of 0-2PI.
	 * @param radians The source angle. 
	 * @return The normalized angle. 
	 **/
	static float WrapRadians(const float &radians);

	/**
	 * Rounds a value to a amount of places after the decimal point.
	 * @param value The value to round. 
	 * @param place How many places after the decimal to round to. 
	 * @return The rounded value. 
	 **/
	static float RoundToPlace(const float &value, const int32_t &place);

	/**
	 * Used to floor the value if less than the min.
	 * @param min The minimum value. 
	 * @param value The value. 
	 * @return Returns a value with deadband applied. 
	 **/
	static float Deadband(const float &min, const float &value);

	/**
	 * Checks if two values are almost equal.
	 * @param a The first value. 
	 * @param b The second value. 
	 * @param eps EPS is the measure of equality. 
	 * @return If both are almost equal. 
	 **/
	static bool AlmostEqual(const float &a, const float &b, const float &eps);

	/**
	 * Gradually changes a value to a target.
	 * @param current The current value. 
	 * @param target The target value. 
	 * @param rate The rate to go from current to the target. 
	 * @return The changed value. 
	 **/
	static float SmoothDamp(const float &current, const float &target, const float &rate);

	/**
	 * Interpolates two values by a factor using linear interpolation.
	 * @param a The first value. 
	 * @param b The second value. 
	 * @param factor The factor value. 
	 * @return Returns a interpolation value. 
	 **/
	static float Lerp(const float &a, const float &b, const float &factor);

	/**
	 * Interpolates two values by a factor using cosine interpolation.
	 * @param a The first value. 
	 * @param b The second value. 
	 * @param factor The blend value. 
	 * @return Returns a interpolated value. 
	 **/
	static float CosLerp(const float &a, const float &b, const float &factor);

	/**
	 * A calculation that steps smoothly between two edges.
	 * @param edge0 The inner edge. 
	 * @param edge1 The outer edge. 
	 * @param x The sample. 
	 * @return The resulting stepped value. 
	 **/
	static float SmoothlyStep(const float &edge0, const float &edge1, const float &x);

	/**
	 * Takes the cosign of a number by using the sign and a additional angle.
	 * @param sin The sin. 
	 * @param angle The angle. 
	 * @return The resulting cosign. 
	 **/
	static float CosFromSin(const float &sin, const float &angle);

	/**
	 * Combines a seed into a hash and modifies the seed by the new hash.
	 * @param seed The seed. 
	 * @param v The value to hash. 
	 **/
	template<typename T>
	static void HashCombine(std::size_t &seed, const T &v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
};
}
