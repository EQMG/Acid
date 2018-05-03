#include "EventChange.hpp"

namespace fl
{
	template<typename T>
	EventChange<T>::EventChange(T *reference, const std::function<void()> &onEvent) :
		IEvent(),
		m_reference(reference),
		m_onEvent(onEvent)
	{
	}

	template<typename T>
	bool EventChange<T>::EventTriggered()
	{
		T newValue = &m_reference;

		if (newValue == nullptr)
		{
			return false;
		}

		const bool triggered = newValue != m_current;
		m_current = newValue;
		return triggered;
	}

	template<typename T>
	void EventChange<T>::OnEvent()
	{
		m_onEvent();
	}
}
