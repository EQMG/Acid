#pragma once

#include <functional>
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
	private:
		Timer m_timer;
		bool m_repeat;
		std::function<void()> m_onEvent;
	public:
		/// <summary>
		/// Creates a new time event.
		/// </summary>
		/// <param name="interval"> The amount of seconds in the future to run the event. </param>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		EventTime(const float &interval, const bool &repeat, const std::function<void()> &onEvent);

		/// <summary>
		/// Deconstructor for the timed event.
		/// </summary>
		~EventTime();

		bool EventTriggered() override;

		void OnEvent() override;

		bool RemoveAfterEvent() override { return !m_repeat; }
	};
}
