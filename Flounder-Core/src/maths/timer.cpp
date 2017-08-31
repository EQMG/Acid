#include "timer.hpp"
#include <iostream>

namespace Flounder
{
	timer::timer(const float &interval) :
		m_startTime(0.0f),
		m_interval(interval * 1000.0f)
	{
		if (Engine::Get() != nullptr)
		{
			m_startTime = Engine::Get()->GetTimeMs();
		}
	}

	timer::~timer()
	{
	}

	bool timer::isPassedTime() const
	{
		return Engine::Get()->GetTimeMs() - m_startTime >= m_interval;
	}

	void timer::resetStartTime()
	{
		m_startTime = Engine::Get()->GetTimeMs();
	}

	float timer::getInterval() const
	{
		return m_interval / 1000.0f;
	}

	void timer::setInterval(const float &interval)
	{
		if (m_interval == interval * 1000.0f)
		{
			return;
		}

		m_interval = interval * 1000.0f;
		m_startTime = Engine::Get()->GetTimeMs();
	}
}
