#pragma once

namespace flounder {
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
		/// <param name="timeMs"> The current time in ms. </param>
		timer(double interval, float timeMs);

		/// <summary>
		/// Deconstructor for the timer.
		/// </summary>
		~timer();

		/// <summary>
		/// Gets if the interval has been passes for the timer.
		/// </summary>
		/// <returns> If the interval was exceeded. </returns>
		/// <param name="timeMs"> The current time in ms. </param>
		bool isPassedTime(float timeMs);

		/// <summary>
		/// Adds the intervals value to the start time.
		/// </summary>
		/// <param name="timeMs"> The current time in ms. </param>
		void resetStartTime(float timeMs);

		/// <summary>
		/// Gets what the interval is. (Seconds).
		/// </summary>
		/// <returns> The timers current interval. </returns>
		double getInterval();

		/// <summary>
		/// Gets the timers interval. (Seconds, Resets timer).
		/// </summary>
		/// <param name="interval"> The new timer interval. </param>
		/// <param name="timeMs"> The current time in ms. </param>
		void setInterval(double interval, float timeMs);
	};
}
