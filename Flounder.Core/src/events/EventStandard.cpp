#include "EventStandard.hpp"

namespace Flounder
{
	EventStandard::EventStandard(const bool &repeat, const std::function<bool()> &triggered, const std::function<void()> &onEvent) :
		IEvent(),
		m_repeat(repeat),
		m_triggered(triggered),
		m_onEvent(onEvent)
	{
	}

	EventStandard::EventStandard(const std::function<bool()> &triggered, const std::function<void()> &onEvent)
	{
		m_repeat = true;
		m_triggered = triggered;
		m_onEvent = onEvent;
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
