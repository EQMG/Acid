#include "EventChange.hpp"

namespace acid
{
	template<typename T>
	EventChange<T>::EventChange(T *reference, const std::function<void()> &onEvent, const bool &repeat) :
		m_reference(reference),
		m_onEvent(onEvent),
		m_repeat(repeat)
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
