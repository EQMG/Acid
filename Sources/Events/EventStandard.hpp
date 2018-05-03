#pragma once

#include <functional>
#include "IEvent.hpp"

namespace fl
{
	/// <summary>
	/// A class that is the most basic implementation of the event interface.
	/// </summary>
	class FL_EXPORT EventStandard :
		public IEvent
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
		EventStandard(const bool &repeat, const std::function<bool()> &triggered, const std::function<void()> &onEvent);

		/// <summary>
		/// Creates a new standard event that repeats.
		/// </summary>
		/// <param name="triggered"> A function called to check if the event was triggered. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		EventStandard(const std::function<bool()> &triggered, const std::function<void()> &onEvent);

		bool EventTriggered() override;

		void OnEvent() override;

		bool RemoveAfterEvent() override { return !m_repeat; }
	};
}
