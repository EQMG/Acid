#include "timer.h"

namespace flounder {
	timer::timer(double interval, float timeMs)
	{
		m_startTime = timeMs;
		m_interval = m_interval * 1000.0;
	}

	timer::~timer()
	{
	//	delete &m_startTime;
	//	delete &m_interval;
	}

	bool timer::isPassedTime(float timeMs)
	{
		return timeMs - m_startTime > m_interval;
	}

	void timer::resetStartTime(float timeMs)
	{
		this->m_startTime = timeMs;
	}

	double timer::getInterval()
	{
		return m_interval / 1000.0;
	}

	void timer::setInterval(double interval, float timeMs)
	{
		this->m_interval = interval * 1000.0;
		this->m_startTime = timeMs;
	}
}
