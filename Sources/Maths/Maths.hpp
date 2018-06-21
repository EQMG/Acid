#pragma once

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "Engine/Exports.hpp"

#define PI 3.14159265358979323846f

namespace fl
{
	/// <summary>
	/// A class that holds many various math functions.
	/// </summary>
	class FL_EXPORT Maths
	{
	public:
		/// <summary>
		/// Generates a random value from between a range.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value. </param>
		/// <returns> The randomly selected value within the range. </returns>
		static float Random(const float &min = 0.0f, const float &max = 1.0f);

		/// <summary>
		/// Generates a single value from a normal distribution, using Box-Muller.
		/// https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
		/// </summary>
		/// <param name="standardDeviation"> The standards deviation of the distribution. </param>
		/// <param name="mean"> The mean of the distribution. </param>
		/// <returns> A normally distributed value. </returns>
		static float RandomNormal(const float &standardDeviation, const float &mean);

		/// <summary>
		/// Creates a number between two numbers, logarithmic.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value. </param>
		/// <returns> The final random number. </returns>
		static float RandomLog(const float &min, const float &max);

		/// <summary>
		/// Converts degrees to radians.
		/// </summary>
		/// <param name="degrees"> The degrees value. </param>
		/// <returns> The radians value. </returns>
		static float Radians(const float &degrees);

		/// <summary>
		/// Converts radians to degrees.
		/// </summary>
		/// <param name="radians"> The radians value. </param>
		/// <returns> The degrees value. </returns>
		static float Degrees(const float &radians);

		/// <summary>
		/// Normalizes a angle into the range of 0-360.
		/// </summary>
		/// <param name="degrees"> The source angle. </param>
		/// <returns> The normalized angle. </returns>
		static float NormalizeDegrees(const float &degrees);

		/// <summary>
		/// Normalizes a angle into the range of 0-2PI.
		/// </summary>
		/// <param name="radians"> The source angle. </param>
		/// <returns> The normalized angle. </returns>
		static float NormalizeRadians(const float &radians);

		/// <summary>
		/// Rounds a value to a amount of places after the decimal point.
		/// </summary>
		/// <param name="value"> The value to round. </param>
		/// <param name="place"> How many places after the decimal to round to. </param>
		/// <returns> The rounded value. </returns>
		static float RoundToPlace(const float &value, const int &place);

		/// <summary>
		/// Used to floor the value if less than the min.
		/// </summary>
		/// <param name="min"> The minimum value. </param>
		/// <param name="value"> The value. </param>
		/// <returns> Returns a value with deadband applied. </returns>
		static float Deadband(const float &min, const float &value);

		/// <summary>
		/// Ensures {@code value} is in the range of {@code min} to {@code max}. If {@code value} is greater than {@code max}, this will return {@code max}. If {@code value} is less than {@code min}, this will return {@code min}. Otherwise, {@code value} is returned unchanged.
		/// </summary>
		/// <param name="value"> The value to clamp. </param>
		/// <param name="min"> The smallest value of the result. </param>
		/// <param name="max"> The largest value of the result. </param>
		/// <returns> {@code value}, clamped between {@code min} and {@code max}. </returns>
		static float Clamp(const float &value, const float &min, const float &max);

		/// <summary>
		/// Limits the value.
		/// </summary>
		/// <param name="value"> The value. </param>
		/// <param name="limit"> The limit. </param>
		/// <returns> A limited value. </returns>
		static float Limit(const float &value, const float &limit);

		/// <summary>
		/// Checks if two values are almost equal.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="eps"> EPS is the measure of equality. </param>
		/// <returns> If both are almost equal. </returns>
		static bool AlmostEqual(const float &a, const float &b, const float &eps);

		/// <summary>
		/// Gradually changes a value to a target.
		/// </summary>
		/// <param name="current"> The current value. </param>
		/// <param name="target"> The target value. </param>
		/// <param name="rate"> The rate to go from current to the target. </param>
		/// <returns> The changed value. </returns>
		static float SmoothDamp(const float &current, const float &target, const float &rate);

		/// <summary>
		/// Interpolates two values by a blendFactor using linear interpolation.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="blend"> The blend value. </param>
		/// <returns> Returns a interpolated value. </returns>
		static float Interpolate(const float &a, const float &b, const float &blend);

		/// <summary>
		/// Interpolates two values by a blendFactor using cosine interpolation.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="blend"> The blend value. </param>
		/// <returns> Returns a interpolated value. </returns>
		static float InterpolateCosine(const float &a, const float &b, const float &blend);

		/// <summary>
		/// A calculation that steps smoothly between two edges.
		/// </summary>
		/// <param name="edge0"> The inner edge. </param>
		/// <param name="edge1"> The outer edge. </param>
		/// <param name="x"> The sample. </param>
		/// <returns> The resulting stepped value. </returns>
		static float SmoothlyStep(const float &edge0, const float &edge1, const float &x);
	};
}
