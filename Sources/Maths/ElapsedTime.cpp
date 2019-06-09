#include "ElapsedTime.hpp"

namespace acid
{
ElapsedTime::ElapsedTime(const Time &interval) :
	m_startTime{Time::Now()},
	m_interval{interval}
{
}

uint32_t ElapsedTime::GetElapsed()
{
	auto now{Time::Now()};
	auto elapsed{static_cast<uint32_t>(std::floor((now - m_startTime) / m_interval))};

	if (elapsed != 0.0f)
	{
		m_startTime = now;
	}

	return elapsed;
}

const Metadata &operator>>(const Metadata &metadata, ElapsedTime &elapsedTime)
{
	metadata >> elapsedTime.m_interval;
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ElapsedTime &elapsedTime)
{
	metadata << elapsedTime.m_interval;
	return metadata;
}
}
