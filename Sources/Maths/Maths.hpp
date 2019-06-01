#pragma once

#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief class  that holds many various math functions.
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
	 * @tparam T The values type.
	 * @param degrees The degrees value. 
	 * @return The radians value. 
	 **/
	template<typename T = float>
	static T Radians(const T &degrees)
	{
		return degrees * static_cast<T>(DegToRad);
	}

	/**
	 * Converts radians to degrees.
	 * @tparam T The values type.
	 * @param radians The radians value. 
	 * @return The degrees value. 
	 **/
	template<typename T = float>
	static T Degrees(const T &radians)
	{
		return radians * static_cast<T>(RadToDeg);
	}

	/**
	 * Normalizes a angle into the range of 0-360.
	 * @tparam T The values type.
	 * @param degrees The source angle. 
	 * @return The normalized angle. 
	 **/
	template<typename T = float>
	static T WrapDegrees(const T &degrees)
	{
		auto x = std::fmod(degrees, 360);

		if (x < 0)
		{
			x += 360;
		}

		return static_cast<T>(x);
	}

	/**
	 * Normalizes a angle into the range of 0-2PI.
	 * @tparam T The values type.
	 * @param radians The source angle. 
	 * @return The normalized angle. 
	 **/
	template<typename T = float>
	static T WrapRadians(const T &radians)
	{
		auto x = std::fmod(radians, 2 * Pi);

		if (x < 0)
		{
			x += 2 * Pi;
		}

		return static_cast<T>(x);
	}

	/**
	 * Rounds a value to a amount of places after the decimal point.
	 * @tparam T The values type.
	 * @param value The value to round. 
	 * @param place How many places after the decimal to round to. 
	 * @return The rounded value. 
	 **/
	template<typename T = float>
	static T RoundToPlace(const T &value, const int32_t &place)
	{
		auto placeMul = std::pow(static_cast<T>(10), static_cast<T>(place));
		return static_cast<T>(std::round(value * placeMul) / placeMul);
	}

	/**
	 * Used to floor the value if less than the min.
	 * @tparam T The values type.
	 * @param min The minimum value.
	 * @param value The value. 
	 * @return Returns a value with deadband applied. 
	 **/
	template<typename T = float>
	static T Deadband(const T &min, const T &value)
	{
		return std::fabs(value) >= std::fabs(min) ? value : 0.0f;
	}

	/**
	 * Checks if two values are almost equal.
	 * @tparam T The values type.
	 * @tparam K The EPS type.
	 * @param a The first value. 
	 * @param b The second value. 
	 * @param eps EPS is the measure of equality. 
	 * @return If both are almost equal. 
	 **/
	template<typename T = float, typename K = float>
	static bool AlmostEqual(const T &a, const T &b, const K &eps)
	{
		return std::fabs(a - b) < eps;
	}

	/**
	 * Gradually changes a value to a target.
	 * @tparam T The current and target types.
	 * @tparam K The rate type.
	 * @param current The current value. 
	 * @param target The target value. 
	 * @param rate The rate to go from current to the target. 
	 * @return The changed value. 
	 **/
	template<typename T = float, typename K = float>
	static auto SmoothDamp(const T &current, const T &target, const K &rate)
	{
		return current + ((target - current) * rate);
	}

	/**
	 * Interpolates two values by a factor using linear interpolation.
	 * @tparam T The value types.
	 * @tparam K The factor type.
	 * @param a The first value. 
	 * @param b The second value. 
	 * @param factor The factor value. 
	 * @return Returns a interpolation value. 
	 **/
	template<typename T = float, typename K = float>
	static auto Lerp(const T &a, const T &b, const K &factor)
	{
		return a * (1 - factor) + b * factor;
	}

	/**
	 * Interpolates two values by a factor using cosine interpolation.
	 * @tparam T The value types.
	 * @tparam K The factor type.
	 * @param a The first value. 
	 * @param b The second value. 
	 * @param factor The blend value. 
	 * @return Returns a interpolated value. 
	 **/
	template<typename T = float, typename K = float>
	static auto CosLerp(const T &a, const T &b, const K &factor)
	{
		auto ft = factor * Pi;
		auto f = 1 - std::cos(ft) / 2;
		return (a * (1 - f)) + (b * f);
	}

	/**
	 * A calculation that steps smoothly between two edges.
	 * @tparam T The edge types.
	 * @tparam K The sample type.
	 * @param edge0 The inner edge. 
	 * @param edge1 The outer edge. 
	 * @param x The sample. 
	 * @return The resulting stepped value. 
	 **/
	template<typename T = float, typename K = float>
	static auto SmoothlyStep(const T &edge0, const T &edge1, const K &x)
	{
		auto s = std::clamp((x - edge0) / (edge1 - edge0), 0, 1);
		return s * s * (3 - 2 * s);
	}

	/**
	 * Takes the cosign of a number by using the sign and a additional angle.
	 * @tparam T The sin type.
	 * @tparam K The angle type.
	 * @param sin The sin. 
	 * @param angle The angle. 
	 * @return The resulting cosign. 
	 **/
	template<typename T = float, typename K = float>
	static auto CosFromSin(const T &sin, const K &angle)
	{
		// sin(x)^2 + cos(x)^2 = 1
		auto cos = std::sqrt(1 - sin * sin);
		auto a = angle + (Pi / 2);
		auto b = a - static_cast<int32_t>(a / (2 * Pi)) * (2 * Pi);

		if (b < 0)
		{
			b = (2 * Pi) + b;
		}

		if (b >= Pi)
		{
			return -cos;
		}

		return cos;
	};

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
