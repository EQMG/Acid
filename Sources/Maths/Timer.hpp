#pragma once

#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// A timer implementation for events.
	/// </summary>
	class ACID_EXPORT Timer
	{
	private:
		float m_startTime;
		float m_interval;
	public:
		/// <summary>
		/// Creates a new timer.
		/// </summary>
		/// <param name="interval"> The time between events (seconds). </param>
		Timer(const float &interval);

		~Timer();

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
		/// Gets what the interval is. (Seconds).
		/// </summary>
		/// <returns> The timers current interval. </returns>
		float GetInterval() const;

		/// <summary>
		/// Gets the timers interval. (Seconds, Resets timer).
		/// </summary>
		/// <param name="interval"> The new timer interval. </param>
		void SetInterval(const float &interval);
	};
}
