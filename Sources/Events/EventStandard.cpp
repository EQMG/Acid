#include "EventStandard.hpp"

namespace acid
{
EventStandard::EventStandard(std::function<void()> onEvent, std::function<bool()> triggered, const bool& repeat) : m_onEvent(std::move(onEvent)), m_triggered(std::move(triggered)), m_repeat(repeat) {}

bool EventStandard::EventTriggered()
{
	return m_triggered();
}

void EventStandard::OnEvent()
{
	return m_onEvent();
}
}
