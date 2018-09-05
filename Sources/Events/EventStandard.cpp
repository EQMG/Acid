#include "EventStandard.hpp"

namespace acid
{
	EventStandard::EventStandard(const std::function<bool()> &triggered, const std::function<void()> &onEvent, const bool &repeat) :
		m_triggered(triggered),
		m_onEvent(onEvent),
		m_repeat(repeat)
	{
	}

	bool EventStandard::EventTriggered()
	{
		return m_triggered();
	}

	void EventStandard::OnEvent()
	{
		return m_onEvent();
	}
}
