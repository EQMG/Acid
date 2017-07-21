#pragma once

#include <functional>

#include "ievent.h"

namespace flounder
{
	/// <summary>
	/// A class that is the most basic implementation of the event interface.
	/// </summary>
	class eventstandard :
		public ievent
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
		eventstandard(const bool &repeat, const std::function<bool()> &triggered, const std::function<void()> &onEvent);

		/// <summary>
		/// Creates a new standard event that repeats.
		/// </summary>
		/// <param name="triggered"> A function called to check if the event was triggered. </param>
		/// <param name="onEvent"> A function called when the event is triggered. </param>
		eventstandard(const std::function<bool()> &triggered, const std::function<void()> &onEvent);

		bool eventTriggered() override;

		void onEvent() override;

		inline bool removeAfterEvent() override { return !m_repeat; }
	};
}
