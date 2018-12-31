#include "EventTime.hpp"

namespace acid
{
	EventTime::EventTime(const std::function<void()> &onEvent, const Time &interval, const bool &repeat) :
		IEvent(),
		m_onEvent(onEvent),
		m_timer(Timer(interval)),
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
