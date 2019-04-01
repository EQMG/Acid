#pragma once

#include "IEvent.hpp"

namespace acid
{
/**
 * Class that acts as a basic change listener for a value.
 * @tparam T The type of value to find change with.
 */
template<typename T>
class EventChange :
	public IEvent
{
public:
	/**
	 * Creates a new change event.
	 * @param onEvent A function called when the event is triggered.
	 * @param reference The reference function to get the value from.
	 * @param repeat If the event will repeat after the first run.
	 */
	EventChange(const std::function<void(T)> &onEvent, const std::function<T()> &reference, const bool &repeat = true) :
		m_onEvent(onEvent),
		m_reference(reference),
		m_current(m_reference()),
		m_repeat(repeat)
	{
	}

	/**
	 * Creates a new change event.
	 * @param onEvent A function called when the event is triggered.
	 * @param reference The reference to listen to.
	 * @param repeat If the event will repeat after the first run.
	 */
	EventChange(const std::function<void(T)> &onEvent, T *reference, const bool &repeat = true) :
		m_onEvent(onEvent),
		m_reference([reference]() -> T
		{
			return *reference;
		}),
		m_current(m_reference()),
		m_repeat(repeat)
	{
	}

	bool EventTriggered() override
	{
		auto newValue = m_reference();
		bool triggered = newValue != m_current;
		m_current = newValue;
		return triggered;
	}

	void OnEvent() override
	{
		m_onEvent(m_current);
	}

	bool RemoveAfterEvent() override { return !m_repeat; }

private:
	std::function<void(T)> m_onEvent;
	std::function<T()> m_reference;
	T m_current;
	bool m_repeat;
};
}
