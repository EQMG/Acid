#include "EventTime.hpp"

namespace acid
{
	EventTime::EventTime(const float &interval, const bool &repeat, const std::function<void()> &onEvent) :
		IEvent(),
		m_timer(Timer(interval)),
		m_repeat(repeat),
		m_onEvent(onEvent)
	{
	}

	EventTime::~EventTime()
	{
	}

	bool EventTime::EventTriggered()
	{
		if (m_timer.IsPassedTime())
		{
			m_timer.ResetStartTime();
			return true;
		}

		return false;
	}

	void EventTime::OnEvent()
	{
		m_onEvent();
	}
}
