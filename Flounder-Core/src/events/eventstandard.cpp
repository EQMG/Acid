#include "eventstandard.hpp"

namespace flounder
{
	eventstandard::eventstandard(const bool &repeat, const std::function<bool()> &triggered, const std::function<void()> &onEvent) :
		ievent(),
		m_repeat(repeat),
		m_triggered(triggered),
		m_onEvent(onEvent)
	{
	}

	eventstandard::eventstandard(const std::function<bool()> &triggered, const std::function<void()> &onEvent)
	{
		m_repeat = true;
		m_triggered = triggered;
		m_onEvent = onEvent;
	}

	bool eventstandard::eventTriggered()
	{
		return m_triggered();
	}

	void eventstandard::onEvent()
	{
		return m_onEvent();
	}
}
