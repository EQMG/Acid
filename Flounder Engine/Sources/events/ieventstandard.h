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

	public:
		/// <summary>
		/// Creates a new standard event.
		/// </summary>
		/// <param name="repeat"> If the event will repeat after the first run. </param>
		ieventstandard(bool repeat)
		{
			m_repeat = repeat;
		}

		/// <summary>
		/// Creates a new standard event that repeats.
		/// </summary>
		ieventstandard()
		{
			m_repeat = true;
		}

		virtual bool eventTriggered() override = 0;

		virtual void onEvent() override = 0;

		inline bool removeAfterEvent() override { return !m_repeat; }
	};
}
