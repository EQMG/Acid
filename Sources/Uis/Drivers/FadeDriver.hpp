#pragma once

#include "UiDriver.hpp"

namespace acid {
/**
 * @brief A driver that fades from start to end.
 * @tparam T The type to be driven.
 */
template<typename T>
class FadeDriver : public UiDriver<T> {
public:
	/**
	 * Creates a new fade driver.
	 * @param start The fade start interval (0.0-1.0).
	 * @param end The fade end interval (0.0-1.0).
	 * @param peak The peak value.
	 * @param length The time taken to get to the end.
	 */
	FadeDriver(const T &start, const T &end, const T &peak, const Time &length) :
		UiDriver<T>(length),
		start(start),
		end(end),
		peak(peak) {
	}

	/**
	 * Gets the start interval.
	 * @return The start interval.
	 */
	const T &GetStart() const { return start; }
	/**
	 * Sets the start interval (0.0-1.0).
	 * @param start The new start interval.
	 */
	void SetStart(const T &start) { this->start = start; }

	/**
	 * Gets the end interval.
	 * @return The end interval.
	 */
	const T &GetEnd() const { return end; }
	/**
	 * Sets the end interval (0.0-1.0).
	 * @param end The new end interval.
	 */
	void SetEnd(const T &end) { this->end = end; }

	/**
	 * Gets the peak value.
	 * @return The peak value.
	 */
	const T &GetPeak() const { return peak; }
	/**
	 * Sets the peak value.
	 * @param peak The new peak value.
	 */
	void SetPeak(const T &peak) { this->peak = peak; }

protected:
	T Calculate(float factor) override {
		if (factor < start) {
			return factor / start * peak;
		}

		if (factor > end) {
			return (1.0f - (factor - end) / (1.0f - end)) * peak;
		}

		return peak;
	}

private:
	T start;
	T end;
	T peak;
};
}
