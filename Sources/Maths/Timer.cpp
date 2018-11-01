#include "Timer.hpp"

#include "Engine/Engine.hpp"

namespace acid
{
	Timer::Timer(const Time &interval) :
		m_startTime(Engine::GetTime()),
		m_interval(interval)
	{
	}

	bool Timer::IsPassedTime() const
	{
		return Engine::GetTime() - m_startTime >= m_interval;
	}

	void Timer::ResetStartTime()
	{
		m_startTime = Engine::GetTime();
	}

	Time Timer::GetInterval() const
	{
		return m_interval;
	}

	void Timer::SetInterval(const Time &interval)
	{
		if (m_interval == interval)
		{
			return;
		}

		m_interval = interval;
		m_startTime = Engine::GetTime();
	}
}
