#pragma once

#include "Driver.hpp"

namespace acid {
/**
 * @brief A driver that fades from start to end.
 * @tparam T The type to be driven.
 */
template<typename T>
class DriverFade :
	public Driver<T> {
public:
	/**
	 * Creates a new fade driver.
	 * @param start The fade start interval (0.0-1.0).
	 * @param end The fade end interval (0.0-1.0).
	 * @param peak The peak value.
	 * @param length The time taken to get to the end.
	 */
	DriverFade(const T &start, const T &end, const T &peak, const Time &length) :
		Driver<T>(length),
		m_start(start),
		m_end(end),
		m_peak(peak) {
	}

	/**
	 * Gets the start interval.
	 * @return The start interval.
	 */
	const T &GetStart() const { return m_start; }

	/**
	 * Sets the start interval (0.0-1.0).
	 * @param start The new start interval.
	 */
	void SetStart(const T &start) { m_start = start; }

	/**
	 * Gets the end interval.
	 * @return The end interval.
	 */
	const T &GetEnd() const { return m_end; }

	/**
	 * Sets the end interval (0.0-1.0).
	 * @param end The new end interval.
	 */
	void SetEnd(const T &end) { m_end = end; }

	/**
	 * Gets the peak value.
	 * @return The peak value.
	 */
	const T &GetPeak() const { return m_peak; }

	/**
	 * Sets the peak value.
	 * @param peak The new peak value.
	 */
	void SetPeak(const T &peak) { m_peak = peak; }

protected:
	T Calculate(float factor) override {
		if (factor < m_start) {
			return factor / m_start * m_peak;
		}

		if (factor > m_end) {
			return (1.0f - (factor - m_end) / (1.0f - m_end)) * m_peak;
		}

		return m_peak;
	}

private:
	T m_start;
	T m_end;
	T m_peak;
};
}
