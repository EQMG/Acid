#pragma once

#include "Maths/Maths.hpp"
#include "UiDriver.hpp"

namespace acid {
/**
 * @brief A bounce driver that uses a sine wave.
 * @tparam T The type to be driven.
 */
template<typename T>
class BounceDriver : public UiDriver<T> {
public:
	/**
	 * Creates a new sine wave driver.
	 * @param start The start value.
	 * @param end The end value.
	 * @param length The length between two waves.
	 */
	BounceDriver(const T &start, const T &end, const Time &length) :
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
		auto value = 0.5f + std::sin(Maths::PI<float> * 2.0f * factor) * 0.5f;

		if (UiDriver<T>::actualTime > UiDriver<T>::length / 2.0f) {
			value = 0.0f;
		}

		return start + value * (end - start);
	}

private:
	T start;
	T end;
};
}
