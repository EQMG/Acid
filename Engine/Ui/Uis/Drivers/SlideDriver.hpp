#pragma once

#include "UiDriver.hpp"

namespace acid {
/**
 * @brief A driver that slides to its destination using cosine interpolation.
 * @tparam T The type to be driven.
 */
template<typename T>
class SlideDriver : public UiDriver<T> {
public:
	/**
	 * Creates a new slide driver.
	 * @param start The start value.
	 * @param end The end value.
	 * @param length The time to get to the end value.
	 */
	SlideDriver(const T &start, const T &end, const Time &length) :
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
	 * @return The end time.
	 */
	const T &GetEnd() const { return end; }
	/**
	 * Sets the end time.
	 * @param end The new end time.
	 */
	void SetEnd(const T &end) { this->end = end; }

protected:
	T Calculate(float factor) override {
		auto realTime = static_cast<float>(std::min(UiDriver<T>::actualTime, UiDriver<T>::GetLength()) / UiDriver<T>::GetLength());
		return start + realTime * (end - start);
		//return Maths::CosInterpolate(start, end, realTime);
	}

private:
	T start;
	T end;
};
}
