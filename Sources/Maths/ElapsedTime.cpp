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

const Node &operator>>(const Node &node, ElapsedTime &elapsedTime)
{
	node >> elapsedTime.m_interval;
	return node;
}

Node &operator<<(Node &node, const ElapsedTime &elapsedTime)
{
	node << elapsedTime.m_interval;
	return node;
}
}
