#pragma once

#include "../framework/framework.h"

namespace flounder 
{
	/// <summary>
	/// A timer implementation for events.
	/// </summary>
	class timer
	{
	private:
		double m_startTime;
		double m_interval;

	public:
		/// <summary>
		/// Creates a new timer.
		/// </summary>
		/// <param name="interval"> The time between events (seconds). </param>
		timer(const double &interval);

		/// <summary>
		/// Deconstructor for the timer.
		/// </summary>
		~timer();

		/// <summary>
		/// Gets if the interval has been passes for the timer.
		/// </summary>
		/// <returns> If the interval was exceeded. </returns>
		bool isPassedTime();

		/// <summary>
		/// Adds the intervals value to the start time.
		/// </summary>
		void resetStartTime();

		/// <summary>
		/// Gets what the interval is. (Seconds).
		/// </summary>
		/// <returns> The timers current interval. </returns>
		double getInterval();

		/// <summary>
		/// Gets the timers interval. (Seconds, Resets timer).
		/// </summary>
		/// <param name="interval"> The new timer interval. </param>
		void setInterval(const double &interval);
	};
}
