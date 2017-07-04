#pragma once

/// <summary>
/// A timer implementation for events.
/// </summary>
class timer
{
public:
	double m_startTime;
	double m_interval;

	/// <summary>
	/// Creates a new timer.
	/// </summary>
	/// <param name="interval"> The time between events (seconds). </param>
	timer(float interval);

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
};
