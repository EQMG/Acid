#include "EventTime.hpp"

namespace acid
{
EventTime::EventTime(std::function<void()> onEvent, const Time& interval, const bool& repeat) : m_onEvent(std::move(onEvent)), m_timer(interval), m_repeat(repeat) {}

bool EventTime::EventTriggered()
{
	if(m_timer.IsPassedTime())
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
