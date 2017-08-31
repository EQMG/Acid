#include "EventChange.hpp"

namespace Flounder
{
	template<typename t>
	EventChange<t>::EventChange(t *reference, const std::function<void()> &onEvent) :
		IEvent(),
		m_reference(reference),
		m_onEvent(onEvent)
	{
	}

	template<typename t>
	bool EventChange<t>::EventTriggered()
	{
		t newValue = &m_reference;

		if (newValue == nullptr)
		{
			return false;
		}

		bool triggered = newValue != m_current;
		m_current = newValue;
		return triggered;
	}

	template<typename t>
	void EventChange<t>::OnEvent()
	{
		m_onEvent();
	}
}
