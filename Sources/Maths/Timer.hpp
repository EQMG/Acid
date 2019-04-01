#pragma once

#include "Time.hpp"

namespace acid
{
/**
 * @brief A timer implementation for events.
 */
class ACID_EXPORT Timer
{
public:
	/**
	 * Creates a new timer.
	 * @param interval The time between events.
	 */
	explicit Timer(const Time &interval);

	/**
	 * Gets if the amount of time between the current time and the start time.
	 * @return The time passed between current time and start time.
	 */
	Time GetDifference() const;

	/**
	 * Gets if the interval has been passes for the timer.
	 * @return If the interval was exceeded.
	 */
	bool IsPassedTime() const;

	/**
	 * Adds the intervals value to the start time.
	 */
	void ResetStartTime();

	/**
	 * Gets what the interval is.
	 * @return The timers current interval.
	 */
	const Time &GetInterval() const { return m_interval; }

	/**
	 * Gets the timers interval (resets timer).
	 * @param interval The new timer interval.
	 */
	void SetInterval(const Time &interval);

private:
	Time m_startTime;
	Time m_interval;
};
}
