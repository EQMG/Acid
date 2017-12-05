#include "EventTime.hpp"

namespace Flounder
{
	EventTime::EventTime(const float &interval, const bool &repeat, const std::function<void()> &onEvent) :
		IEvent(),
		m_timer(new Timer(interval)),
		m_repeat(repeat),
		m_onEvent(onEvent)
	{
	}

	EventTime::~EventTime()
	{
		delete m_timer;
	}

	bool EventTime::EventTriggered()
	{
		if (m_timer->IsPassedTime())
		{
			m_timer->ResetStartTime();
			return true;
		}

		return false;
	}

	void EventTime::OnEvent()
	{
		m_onEvent();
	}
}
