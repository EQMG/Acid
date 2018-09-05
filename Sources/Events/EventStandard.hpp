#pragma once

#include <functional>
#include "IEvent.hpp"

namespace acid
{
	/// <summary>
	/// A class that is the most basic implementation of the event interface.
	/// </summary>
	class ACID_EXPORT EventStandard :
		public IEvent
	{
	private:
		std::function<bool()> m_triggered;
		std::function<void()> m_onEvent;
		bool m_repeat;
	public:
		/// <summary>
		/// Creates a new standard event.
		/// </summary>
		/// <param name="triggered"> A function called to check if the event was triggered. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		EventStandard(const std::function<bool()> &triggered, const std::function<void()> &onEvent, const bool &repeat = false);

		bool EventTriggered() override;

		void OnEvent() override;

		bool RemoveAfterEvent() override { return !m_repeat; }
	};
}
