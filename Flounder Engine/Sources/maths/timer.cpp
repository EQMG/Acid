#include "timer.h"

namespace flounder {
	timer::timer(double interval)
	{
		m_startTime = framework::get()->getTimeMs();
		m_interval = interval * 1000.0;
	}

	timer::~timer()
	{
	}

	bool timer::isPassedTime()
	{
		return framework::get()->getTimeMs() - m_startTime >= m_interval;
	}

	void timer::resetStartTime()
	{
		this->m_startTime = framework::get()->getTimeMs();
	}

	double timer::getInterval()
	{
		return m_interval / 1000.0;
	}

	void timer::setInterval(double interval)
	{
		this->m_interval = interval * 1000.0;
		this->m_startTime = framework::get()->getTimeMs();
	}
}
