#include "eventchange.h"

namespace flounder
{
	template <typename t>
	eventchange<t>::eventchange(t *reference, const std::function<void()> &onEvent) :
		ievent()
	{
		m_reference = reference;
		m_onEvent = onEvent;
	}

	template<typename t>
	bool eventchange<t>::eventTriggered()
	{
		t newValue = &m_reference;

		if (newValue == NULL)
		{
			return false;
		}

		bool triggered = newValue != m_current;
		m_current = newValue;
		return triggered;
	}

	template<typename t>
	inline void eventchange<t>::onEvent()
	{
		m_onEvent();
	}
}