#include "ElapsedTime.hpp"

#include <cmath>

namespace acid {
ElapsedTime::ElapsedTime(const Time &interval) :
	startTime(Time::Now()),
	interval(interval) {
}

uint32_t ElapsedTime::GetElapsed() {
	auto now = Time::Now();
	auto elapsed = static_cast<uint32_t>(std::floor((now - startTime) / interval));

	if (elapsed != 0.0f) {
		startTime = now;
	}

	return elapsed;
}

const Node &operator>>(const Node &node, ElapsedTime &elapsedTime) {
	node >> elapsedTime.interval;
	return node;
}

Node &operator<<(Node &node, const ElapsedTime &elapsedTime) {
	node << elapsedTime.interval;
	return node;
}
}
