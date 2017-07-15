#pragma once

#include "ievent.h"

namespace flounder {
	/// <summary>
	/// A class that is the most basic implementation of the event interface.
	/// </summary>
	class ieventstandard : public ievent
	{
	private:
		bool m_repeat;
		std::function<bool()> m_triggered;
		std::function<void()> m_onEvent;

	public:
		/// <summary>
		/// Creates a new standard event.
		/// </summary>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		/// <param name="triggered"> A function called to check if the event was triggered. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		ieventstandard(const bool &repeat, const std::function<bool()> &triggered, const std::function<void()> &onEvent)
		{
			m_repeat = repeat;
			m_triggered = triggered;
			m_onEvent = onEvent;
		}

		/// <summary>
		/// Creates a new standard event that repeats.
		/// </summary>
		/// <param name="triggered"> A function called to check if the event was triggered. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		ieventstandard(const std::function<bool()> &triggered, const std::function<void()> &onEvent)
		{
			m_repeat = true;
			m_triggered = triggered;
			m_onEvent = onEvent;
		}

		bool eventTriggered() override
		{
			return m_triggered();
		}

		void onEvent() override
		{
			return m_onEvent();
		}

		inline bool removeAfterEvent() override { return !m_repeat; }
	};
}
