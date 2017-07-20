#pragma once

#include "../maths/timer.h"

#include "ievent.h"

namespace flounder 
{
	/// <summary>
	/// A class that runs a event after a time has passed.
	/// </summary>
	class eventtime :
		public ievent
	{
	private:
		timer *m_timer;
		bool m_repeat;
		std::function<void()> m_onEvent;
	public:
		/// <summary>
		/// Creates a new time event.
		/// </summary>
		/// <param name="interval"> The amount of seconds in the future to run the event. </param>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		eventtime(const float &interval, const bool &repeat, const std::function<void()> &onEvent);

		/// <summary>
		/// Deconstructor for the timed event.
		/// </summary>
		~eventtime();

		bool eventTriggered() override;

		void onEvent() override;

		inline bool removeAfterEvent() override { return !m_repeat; }
	};
}
