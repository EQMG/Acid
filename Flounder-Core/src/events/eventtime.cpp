#include "eventtime.hpp"

namespace flounder
{
	eventtime::eventtime(const float &interval, const bool &repeat, const std::function<void()> &onEvent) :
		ievent()
	{
		m_timer = new timer(interval);
		m_repeat = repeat;
		m_onEvent = onEvent;
	}

	eventtime::~eventtime()
	{
		delete m_timer;
	}

	bool eventtime::eventTriggered()
	{
		if (m_timer->isPassedTime())
		{
			m_timer->resetStartTime();
			return true;
		}

		return false;
	}

	void eventtime::onEvent()
	{
		m_onEvent();
	}
}
