#pragma once

#include "Time.hpp"

namespace acid {
class ACID_CORE_EXPORT ElapsedTime {
public:
	explicit ElapsedTime(const Time &interval = -1s);

	uint32_t GetElapsed();

	const Time &GetStartTime() const { return startTime; }
	void SetStartTime(const Time &startTime) { this->startTime = startTime; }

	const Time &GetInterval() const { return interval; }
	void SetInterval(const Time &interval) { this->interval = interval; }

	friend const Node &operator>>(const Node &node, ElapsedTime &elapsedTime);
	friend Node &operator<<(Node &node, const ElapsedTime &elapsedTime);

private:
	Time startTime;
	Time interval;
};
}
