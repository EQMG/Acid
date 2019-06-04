#include "Maths.hpp"

#include <random>

namespace acid
{
static std::random_device RANDOM_DEVICE;
static std::mt19937 RANDOM_GENERATOR(RANDOM_DEVICE());

float Maths::Random(const float &min, const float &max)
{
	std::uniform_real_distribution<float> dist{min, max};
	return dist(RANDOM_GENERATOR);
}

float Maths::RandomNormal(const float &standardDeviation, const float &mean)
{
	std::normal_distribution<float> dist{mean, standardDeviation};
	return dist(RANDOM_GENERATOR);
}

float Maths::RandomLog(const float &min, const float &max)
{
	auto logLower = std::log(min);
	auto logUpper = std::log(max);
	auto raw = Random(0.0f, 1.0f);

	auto result = std::exp(raw * (logUpper - logLower) + logLower);

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
}
