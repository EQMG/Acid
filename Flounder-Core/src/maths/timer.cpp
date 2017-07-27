#include "timer.h"

namespace flounder
{
	timer::timer(const double &interval)
	{
		m_startTime = 0.0f;
		m_interval = interval * 1000.0;

		if (framework::get() != NULL)
		{
			m_startTime = framework::get()->getTimeMs();
		}
	}

	timer::~timer()
	{
	}

	bool timer::isPassedTime() const
	{
		return framework::get()->getTimeMs() - m_startTime >= m_interval;
	}

	void timer::resetStartTime()
	{
		this->m_startTime = framework::get()->getTimeMs();
	}

	double timer::getInterval() const
	{
		return m_interval / 1000.0;
	}

	void timer::setInterval(const double &interval)
	{
		if (m_interval == interval * 1000.0)
		{
			return;
		}

		this->m_interval = interval * 1000.0;
		this->m_startTime = framework::get()->getTimeMs();
	}
}
