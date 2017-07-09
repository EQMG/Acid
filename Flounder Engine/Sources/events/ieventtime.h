#pragma once

#include "ievent.h"
#include "../maths/timer.h"

namespace flounder {
	/// <summary>
	/// A class that runs a event after a time has passed.
	/// </summary>
	class ieventtime : public ievent
	{
	private:
		timer *m_timer;
		bool m_repeat;
	public:
		/// <summary>
		/// Creates a new time event.
		/// </summary>
		/// <param name="interval"> The amount of seconds in the future to run the event. </param>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		ieventtime(float interval, bool repeat)
		{
			m_timer = new timer(interval);
			m_repeat = repeat;
		}

		/// <summary>
		/// Deconstructor for the timed event.
		/// </summary>
		~ieventtime()
		{
			delete m_timer;
		}

		bool eventTriggered() override
		{
			if (m_timer->isPassedTime())
			{
				m_timer->resetStartTime();
				return true;
			}

			return false;
		}

		virtual void onEvent() override = 0;

		inline bool removeAfterEvent() override { return !m_repeat; }
	};
}
