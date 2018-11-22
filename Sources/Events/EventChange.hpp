#pragma once

#include <functional>
#include "IEvent.hpp"

namespace acid
{
	/// <summary>
	/// A class that acts as a basic change listener for a value.
	/// </summary>
	/// @param <T> The type of value to find change with. </param>
	template<typename T>
	class EventChange :
		public IEvent
	{
	private:
		std::function<T()> m_reference;
		T m_current;
		std::function<void(T)> m_onEvent;
		bool m_repeat;
	public:
		/// <summary>
		/// Creates a new change event.
		/// </summary>
		/// <param name="reference"> The reference function to get the value from. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		EventChange(const std::function<T()> &reference, const std::function<void(T)> &onEvent, const bool &repeat = true) :
			m_reference(reference),
			m_current(m_reference()),
			m_onEvent(onEvent),
			m_repeat(repeat)
		{
		}

		/// <summary>
		/// Creates a new change event.
		/// </summary>
		/// <param name="reference"> The reference to listen to. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		EventChange(T *reference, const std::function<void(T)> &onEvent, const bool &repeat = true) :
			m_reference([reference]() -> T
			{
				return *reference;
			}),
			m_current(m_reference()),
			m_onEvent(onEvent),
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
	};
}
