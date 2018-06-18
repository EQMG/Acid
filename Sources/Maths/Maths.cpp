#include "Maths.hpp"

#include <chrono>
#include <random>

namespace fl
{
	float Maths::Random(const float &min, const float &max)
	{
		std::mt19937_64 rng;
		uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
		rng.seed(ss);

		float range = max - min;
		std::uniform_real_distribution<double> unif(0, 1);
		float scaled = static_cast<float>(unif(rng));
		scaled *= range;
		return scaled + min; // == (rand.nextDouble() * (max-min)) + min;
	}

	float Maths::LogRandom(const float &min, const float &max)
	{
		float logLower = std::log(min);
		float logUpper = std::log(max);
		float raw = Random(0.0f, 1.0f);

		float result = std::exp(raw * (logUpper - logLower) + logLower);

		if (result < min)
		{
			result = min;
		}
		else if (result > max)
		{
			result = max;
		}

		return result;
	}

	float Maths::RandomNormallyDistributed(const float &standardDeviation, const float &mean)
	{
		float u1 = Random(0.0f, 1.0f);
		float u2 = Random(0.0f, 1.0f);

		float x1 = std::sqrt(-2.0f * std::log(u1));
		float x2 = 2.0f * PI * u2;
		float z1 = x1 * std::sin(x2); // Random normal(0,1)
		return z1 * standardDeviation + mean;
	}

	float Maths::Radians(const float &degrees)
	{
		return degrees / 180.0f * PI;
	}

	float Maths::Degrees(const float &radians)
	{
		return radians * 180.0f / PI;
	}

	float Maths::NormalizeDegrees(const float &degrees)
	{
		float x = std::fmod(degrees, 360.0f);

		if (x < 0.0f)
		{
			x += 360.0f;
		}

		return x;
	}

	float Maths::NormalizeRadians(const float &radians)
	{
		float x = std::fmod(radians, 2.0f * PI);

		if (x < 0.0f)
		{
			x += 2.0f * PI;
		}

		return x;
	}

	float Maths::RoundToPlace(const float &value, const int &place)
	{
		float placeMul = static_cast<float>(std::pow(10.0f, place));
		return std::round(value * placeMul) / placeMul;
	}

	float Maths::Deadband(const float &min, const float &value)
	{
		return std::fabs(value) >= std::fabs(min) ? value : 0.0f;
	}

	float Maths::Clamp(const float &value, const float &min, const float &max)
	{
		return (value < min) ? min : (value > max) ? max : value;
	}

	float Maths::Limit(const float &value, const float &limit)
	{
		return value > limit ? limit : value;
	}

	bool Maths::AlmostEqual(const float &a, const float &b, const float &eps)
	{
		return std::fabs(a - b) < eps;
	}

	float Maths::SmoothDamp(const float &current, const float &target, const float &rate)
	{
		if (rate < 0.0f)
		{
			return target;
		}

		return current + ((target - current) * rate);
	}

	float Maths::Interpolate(const float &a, const float &b, const float &blend)
	{
		return (a * (1.0f - blend)) + (b * blend);
	}

	float Maths::InterpolateCosine(const float &a, const float &b, const float &blend)
	{
		float ft = blend * PI;
		float f = 1.0f - std::cos(ft) * 0.5f;
		return (a * (1.0f - f)) + (b * f);
	}

	float Maths::SmoothlyStep(const float &edge0, const float &edge1, const float &x)
	{
		float s = Clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
		return s * s * (3.0f - 2.0f * s);
	}
}
