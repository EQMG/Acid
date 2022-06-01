#pragma once

#include "UiDriver.hpp"

namespace acid {
/**
 * @brief A driver that linearly increases its value.
 * @tparam T The type to be driven.
 */
template<typename T>
class LinearDriver : public UiDriver<T> {
public:
	/**
	 * Creates a new linear driver.
	 * @param start The start value.
	 * @param end The end value.
	 * @param length The time to go between values.
	 */
	LinearDriver(const T &start, const T &end, const Time &length) :
		UiDriver<T>(length),
		start(start),
		end(end) {
	}

	/**
	 * Gets the start time.
	 * @return The start time.
	 */
	const T &GetStart() const { return start; }
	/**
	 * Sets the start time.
	 * @param start The new start time.
	 */
	void SetStart(const T &start) { this->start = start; }

	/**
	 * Gets the end time.
	 * @return The ebd time.
	 */
	const T &GetEnd() const { return end; }
	/**
	 * Sets the end time.
	 * @param end The new end time.
	 */
	void SetEnd(const T &end) { this->end = end; }

protected:
	T Calculate(float factor) override {
		return start + factor * (end - start);
	}

private:
	T start;
	T end;
};
}
