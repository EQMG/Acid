#include "Timer.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
Timer::Timer(const Time& interval) : m_startTime(Engine::GetTime()), m_interval(interval) {}

Time Timer::GetDifference() const
{
	return Engine::GetTime() - m_startTime;
}

bool Timer::IsPassedTime() const
{
	return GetDifference() >= m_interval;
}

void Timer::ResetStartTime()
{
	m_startTime = Engine::GetTime();
}

void Timer::SetInterval(const Time& interval)
{
	if(m_interval == interval)
		{
			return;
		}

	m_interval = interval;
	m_startTime = Engine::GetTime();
}
}
