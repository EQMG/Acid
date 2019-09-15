#include "Maths.hpp"

#include <random>

namespace acid {
static std::random_device RandomDevice;
static std::mt19937 RandomGenerator(RandomDevice());

float Maths::Random(float min, float max) {
	std::uniform_real_distribution dist(min, max);
	return dist(RandomGenerator);
}

float Maths::RandomNormal(float standardDeviation, float mean) {
	std::normal_distribution dist(mean, standardDeviation);
	return dist(RandomGenerator);
}

float Maths::RandomLog(float min, float max) {
	auto logLower = std::log(min);
	auto logUpper = std::log(max);
	auto raw = Random(0.0f, 1.0f);

	auto result = std::exp(raw * (logUpper - logLower) + logLower);

	if (result < min) {
		result = min;
	} else if (result > max) {
		result = max;
	}

	return result;
}
}
