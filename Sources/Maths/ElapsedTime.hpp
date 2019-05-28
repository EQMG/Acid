#pragma once

#include "Time.hpp"

namespace acid
{
class ElapsedTime
{
public:
	explicit ElapsedTime(const Time &interval = -1s);

	uint32_t GetElapsed();

	const Time &GetStartTime() const { return m_startTime; }

	void SetStartTime(const Time &startTime) { m_startTime = startTime; }

	const Time &GetInterval() const { return m_interval; }

	void SetInterval(const Time &interval) { m_interval = interval; }

	friend const Metadata &operator>>(const Metadata &metadata, ElapsedTime &elapsedTime);

	friend Metadata &operator<<(Metadata &metadata, const ElapsedTime &elapsedTime);

private:
	Time m_startTime;
	Time m_interval;
};
}
