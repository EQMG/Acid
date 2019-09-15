#pragma once

#include "Time.hpp"

namespace acid {
class ACID_EXPORT ElapsedTime {
public:
	explicit ElapsedTime(const Time &interval = -1s);

	uint32_t GetElapsed();

	const Time &GetStartTime() const { return m_startTime; }
	void SetStartTime(const Time &startTime) { m_startTime = startTime; }

	const Time &GetInterval() const { return m_interval; }
	void SetInterval(const Time &interval) { m_interval = interval; }

	friend const Node &operator>>(const Node &node, ElapsedTime &elapsedTime);
	friend Node &operator<<(Node &node, const ElapsedTime &elapsedTime);

private:
	Time m_startTime;
	Time m_interval;
};
}
