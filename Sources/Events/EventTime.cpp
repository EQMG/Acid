#include "EventTime.hpp"

namespace acid
{
	EventTime::EventTime(const Time &interval, const std::function<void()> &onEvent, const bool &repeat) :
		IEvent(),
		m_timer(Timer(interval)),
		m_onEvent(onEvent),
		m_repeat(repeat)
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
