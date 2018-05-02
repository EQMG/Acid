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
		/// Gets a random number.
		/// </summary>
		/// <returns> The random number. </returns>
		static float Random()
		{
			return static_cast<float>(rand()) / RAND_MAX;
		}

		/// <summary>
		/// Creates a number between two numbers, logarithmic.
		/// </summary>
		/// <param name="lowerLimit"> The lower number. </param>
		/// <param name="upperLimit"> The upper number. </param>
		/// <returns> The final random number. </returns>
		static double LogRandom(const double &lowerLimit, const double &upperLimit)
		{
			const double logLower = log(lowerLimit);
			const double logUpper = log(upperLimit);
			const double raw = RandomInRange(0.0f, 1.0f);

			double result = exp(raw * (logUpper - logLower) + logLower);

			if (result < lowerLimit)
			{
				result = lowerLimit;
			}
			else if (result > upperLimit)
			{
				result = upperLimit;
			}

			return result;
		}

		/// <summary>
		/// Generates a single value from a normal distribution, using Box-Muller.
		/// https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
		/// </summary>
		/// <param name="standardDeviation"> The standards deviation of the distribution. </param>
		/// <param name="mean"> The mean of the distribution. </param>
		/// <returns> A normally distributed value. </returns>
		static float NormallyDistributedSingle(const float &standardDeviation, const float &mean)
		{
			// Intentionally duplicated to avoid IEnumerable overhead.
			const double u1 = RandomInRange(0.0, 1.0);
			const double u2 = RandomInRange(0.0, 1.0);

			const double x1 = sqrt(-2.0 * log(u1));
			const double x2 = 2.0 * PI * u2;
			const double z1 = x1 * sin(x2); // Random normal(0,1)
			return static_cast<float>(z1) * standardDeviation + mean;
		}

		/// <summary>
		/// Converts degrees to radians.
		/// </summary>
		/// <param name="degrees"> The degrees value. </param>
		/// <returns> The radians value. </returns>
		template<typename T>
		static T Radians(const T &degrees)
		{
			return degrees / 180.0f * PI;
		}

		/// <summary>
		/// Converts radians to degrees.
		/// </summary>
		/// <param name="radians"> The radians value. </param>
		/// <returns> The degrees value. </returns>
		template<typename T>
		static T Degrees(const T &radians)
		{
			return radians * 180.0f / PI;
		}

		/// <summary>
		/// Normalizes a angle into the range of 0-360.
		/// </summary>
		/// <param name="angle"> The source angle. </param>
		/// <returns> The normalized angle. </returns>
		template<typename T>
		static T NormalizeAngle(const T &angle)
		{
			if (angle >= static_cast<T>(360.0))
			{
				return angle - static_cast<T>(360.0);
			}

			if (angle < static_cast<T>(0.0))
			{
				return angle + static_cast<T>(360.0);
			}

			return angle;
		}

		/// <summary>
		/// Rounds a value to a amounT of places after the decimal point.
		/// </summary>
		/// <param name="value"> The value to round. </param>
		/// <param name="place"> How many places after the decimal to round to. </param>
		/// <returns> The rounded value. </returns>
		template<typename T>
		static T RoundToPlace(const T &value, const int &place)
		{
			T placeMul = static_cast<T>(pow(10, place));
			return round(value * placeMul) / placeMul;
		}

		/// <summary>
		/// Used to floor the value if less than the min.
		/// </summary>
		/// <param name="min"> The minimum value. </param>
		/// <param name="value"> The value. </param>
		/// <returns> Returns a value with deadband applied. </returns>
		template<typename T>
		static T Deadband(const T &min, const T &value)
		{
			return fabs(value) >= fabs(min) ? value : static_cast<T>(0.0);
		}

		/// <summary>
		/// Ensures {@code value} is in the range of {@code min} to {@code max}. If {@code value} is greater than {@code max}, this will return {@code max}. If {@code value} is less than {@code min}, this will return {@code min}. Otherwise, {@code value} is returned unchanged.
		/// </summary>
		/// <param name="value"> The value to clamp. </param>
		/// <param name="min"> The smallest value of the result. </param>
		/// <param name="max"> The largest value of the result. </param>
		/// <returns> {@code value}, clamped between {@code min} and {@code max}. </returns>
		template<typename T>
		static T Clamp(const T &value, const T &min, const T &max)
		{
			return (value < min) ? min : (value > max) ? max : value;
		}

		/// <summary>
		/// Limits the value.
		/// </summary>
		/// <param name="value"> The value. </param>
		/// <param name="limit"> The limit. </param>
		/// <returns> A limited value. </returns>
		template<typename T>
		static T Limit(const T &value, const T &limit)
		{
			return value > limit ? limit : value;
		}

		/// <summary>
		/// Checks if two values are almost equal.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="eps"> EPS is the measure of equality. </param>
		/// <returns> If both are almost equal. </returns>
		template<typename T>
		static bool AlmostEqual(const T &a, const T &b, const T &eps)
		{
			return fabs(a - b) < eps;
		}

		/// <summary>
		/// Gradually changes a value to a target.
		/// </summary>
		/// <param name="current"> The current value. </param>
		/// <param name="target"> The target value. </param>
		/// <param name="rate"> The rate to go from current to the target. </param>
		/// <returns> The changed value. </returns>
		template<typename T>
		static T SmoothDamp(const T &current, const T &target, const T &rate)
		{
			if (rate < 0.0f)
			{
				return target;
			}

			return current + ((target - current) * rate);
		}

		/// <summary>
		/// Interpolates two values by a blendFactor using linear interpolation.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="blend"> The blend value. </param>
		/// <returns> Returns a interpolated value. </returns>
		template<typename T>
		static T Interpolate(const T &a, const T &b, const T &blend)
		{
			return (a * static_cast<T>(1.0f - blend)) + (b * blend);
		}

		/// <summary>
		/// Interpolates two values by a blendFactor using cos interpolation.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="blend"> The blend value. </param>
		/// <returns> Returns a interpolated value. </returns>
		template<typename T>
		static T CosInterpolate(const T &a, const T &b, const T &blend)
		{
			const float ft = static_cast<T>(blend) * PI;
			const float f = 1.0f - std::cos(ft) * 0.5f;
			return (a * static_cast<T>(1.0f - f)) + (b * f);
		}

		/// <summary>
		/// A calculation thaT steps smoothly between two edges.
		/// </summary>
		/// <param name="edge0"> The inner edge. </param>
		/// <param name="edge1"> The outer edge. </param>
		/// <param name="x"> The sample. </param>
		/// <returns> The resulting stepped value. </returns>
		template<typename T>
		static T SmoothlyStep(const T &edge0, const T &edge1, const T &x)
		{
			T s = Clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
			return s * s * (static_cast<T>(3.0 - 2.0) * s);
		}

		/// <summary>
		/// Generates a random value from between a range.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value. </param>
		/// <returns> The randomly selected value within the range. </returns>
		template<typename T>
		static T RandomInRange(const T &min, const T &max)
		{
			T range = max - min;
			float scaled = Random();
			scaled *= static_cast<float>(range);
			return static_cast<T>(scaled) + min; // == (rand.nextDouble() * (max-min)) + min;
		}
	};
}
