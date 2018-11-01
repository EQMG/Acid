#pragma once

#include "Engine/Exports.hpp"
#include "Time.hpp"

namespace acid
{
	/// <summary>
	/// A timer implementation for events.
	/// </summary>
	class ACID_EXPORT Timer
	{
	private:
		Time m_startTime;
		Time m_interval;
	public:
		/// <summary>
		/// Creates a new timer.
		/// </summary>
		/// <param name="interval"> The time between events. </param>
		explicit Timer(const Time &interval);

		/// <summary>
		/// Gets if the interval has been passes for the timer.
		/// </summary>
		/// <returns> If the interval was exceeded. </returns>
		bool IsPassedTime() const;

		/// <summary>
		/// Adds the intervals value to the start time.
		/// </summary>
		void ResetStartTime();

		/// <summary>
		/// Gets what the interval is.
		/// </summary>
		/// <returns> The timers current interval. </returns>
		Time GetInterval() const;

		/// <summary>
		/// Gets the timers interval (resets timer).
		/// </summary>
		/// <param name="interval"> The new timer interval. </param>
		void SetInterval(const Time &interval);
	};
}
