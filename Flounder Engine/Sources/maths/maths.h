#pragma once

#include <iostream>
#include <stdexcept>
#include <cmath>

#define PI 3.14159265358979323846f
#define DEGREES_IN_CIRCLE 360.0f
#define DEGREES_IN_HALF_CIRCLE 180.0f
#define ANG2RAD PI / DEGREES_IN_HALF_CIRCLE
#define LOG_HALF log(0.5f)

#define __radians(a) (a * (PI / DEGREES_IN_HALF_CIRCLE))
#define __degrees(b) (b * (PI / DEGREES_IN_HALF_CIRCLE))
#define __random() (((double) rand()) / RAND_MAX);

namespace flounder {
	/// <summary>
	/// A class that holds many various math functions.
	/// </summary>
	class maths
	{
	public:
		/// <summary>
		/// Does a mod like the GLSL function.
		/// </summary>
		/// <param name="x"> The first value. </param>
		/// <param name="y"> The second value.
		/// </param>
		/// <returns> The resultant mod. </returns>
		template<typename t>
		static t mod(const t &x, const t &y)
		{
			return x - y * floor(x / y);
		}

		/// <summary>
		/// Normalizes a angle into the range of 0-360.
		/// </summary>
		/// <param name="angle"> The source angle.
		/// </param>
		/// <returns> The normalized angle. </returns>
		template<typename t>
		static t normalizeAngle(const t &angle)
		{
			if (angle >= (t) 360.0)
			{
				return angle - (t) 360.0;
			}
			else if (angle < (t) 0.0)
			{
				return angle + (t) 360.0;
			}

			return angle;
		}

		/// <summary>
		/// Rounds a value to a amount of places after the decimal point.
		/// </summary>
		/// <param name="value"> The value to round. </param>
		/// <param name="place"> How many places after the decimal to round to.
		/// </param>
		/// <returns> The rounded value. </returns>
		template<typename t>
		static t roundToPlace(const t &value, const int &place)
		{
			t placeMul = (t) pow(10.0, place);
			return round(value * placeMul) / placeMul;
		}

		/// <summary>
		/// Used to floor the value if less than the min.
		/// </summary>
		/// <param name="min"> The minimum value. </param>
		/// <param name="value"> The value.
		/// </param>
		/// <returns> Returns a value with deadband applied. </returns>
		template<typename t>
		static t deadband(const t &min, const t &value)
		{
			return abs(value) >= abs(min) ? value : (t) 0.0;
		}

		/// <summary>
		/// Ensures {@code value} is in the range of {@code min} to {@code max}. If {@code value} is greater than {@code max}, this will return {@code max}. If {@code value} is less than {@code min}, this will return {@code min}. Otherwise, {@code value} is returned unchanged.
		/// </summary>
		/// <param name="value"> The value to clamp. </param>
		/// <param name="min"> The smallest value of the result. </param>
		/// <param name="max"> The largest value of the result.
		/// </param>
		/// <returns> {@code value}, clamped between {@code min} and {@code max}. </returns>
		template<typename t>
		static t clamp(const t &value, const t &min, const t &max)
		{
			return (value < min) ? min : (value > max) ? max : value;
		}

		/// <summary>
		/// Limits the value.
		/// </summary>
		/// <param name="value"> The value. </param>
		/// <param name="limit"> The limit.
		/// </param>
		/// <returns> A limited value. </returns>
		template<typename t>
		static t limit(const t &value, const t &limit)
		{
			return value > limit ? limit : value;
		}

		/// <summary>
		/// Checks if two values are almost equal.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="eps"> EPS is the measure of equality.
		/// </param>
		/// <returns> If both are almost equal. </returns>
		template<typename t>
		static bool almostEqual(const t &a, const t &b, const t &eps)
		{
			return abs(a - b) < eps;
		}

		/// <summary>
		/// Interpolates two values by a blendFactor using cos interpolation.
		/// </summary>
		/// <param name="a"> The first value. </param>
		/// <param name="b"> The second value. </param>
		/// <param name="blend"> The blend value.
		/// </param>
		/// <returns> Returns a interpolated value. </returns>
		template<typename t>
		static t cosInterpolate(const t &a, const t &b, const t &blend)
		{
			double ft = blend * PI;
			float f = (1.0f - (float) cos(ft)) * 0.5f;
			return a * (1.0f - f) + b * f;
		}

		/// <summary>
		/// A calculation that steps smoothly between two edges.
		/// </summary>
		/// <param name="edge0"> The inner edge. </param>
		/// <param name="edge1"> The outer edge. </param>
		/// <param name="x"> The sample.
		/// </param>
		/// <returns> The resulting stepped value. </returns>
		template<typename t>
		static t smoothlyStep(const t &edge0, const t &edge1, const t &x)
		{
			float s = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
			return s * s * (3.0f - 2.0f * t);
		}

		/// <summary>
		/// Generates a random value from between a range.
		/// </summary>
		/// <param name="min"> The min value. </param>
		/// <param name="max"> The max value.
		/// </param>
		/// <returns> The randomly selected value within the range. </returns>
		static float randomInRange(const float &min, const float &max)
		{
			float range = max - min;
			float scaled = (float) __random();
			scaled *= range;
			return scaled + min; // == (rand.nextDouble() * (max-min)) + min;
		}

		/// <summary>
		/// Creates a number between two numbers, logarithmic.
		/// </summary>
		/// <param name="lowerLimit"> The lower number. </param>
		/// <param name="upperLimit"> The upper number.
		/// </param>
		/// <returns> The final random number. </returns>
		static double logRandom(const double &lowerLimit, const double &upperLimit)
		{
			double logLower = log(lowerLimit);
			double logUpper = log(upperLimit);

			double raw = __random();
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
		/// <param name="mean"> The mean of the distribution.
		/// </param>
		/// <returns> A normally distributed value. </returns>
		static float normallyDistributedSingle(const float &standardDeviation, const float &mean)
		{
			// Intentionally duplicated to avoid IEnumerable overhead.
			double u1 = __random(); // These are uniform(0,1) random doubles.
			double u2 = __random();

			double x1 = sqrt(-2.0 * log(u1));
			double x2 = 2.0 * PI * u2;
			double z1 = x1 * sin(x2); // Random normal(0,1)
			return (float)z1 * standardDeviation + mean;
		}
	};
}
