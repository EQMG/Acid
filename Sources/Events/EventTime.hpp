#pragma once

#include <functional>
#include "Maths/Time.hpp"
#include "Maths/Timer.hpp"
#include "IEvent.hpp"

namespace acid
{
	/// <summary>
	/// A class that runs a event after a time has passed.
	/// </summary>
	class ACID_EXPORT EventTime :
		public IEvent
	{
	public:
		/// <summary>
		/// Creates a new time event.
		/// </summary>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		/// <param name="interval"> The amount of time in the future to run the event. </param>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		EventTime(std::function<void()> onEvent, const Time &interval, const bool &repeat = false);

		bool EventTriggered() override;

		void OnEvent() override;

		bool RemoveAfterEvent() override { return !m_repeat; }
	private:
		std::function<void()> m_onEvent;
		Timer m_timer;
		bool m_repeat;
	};
}
