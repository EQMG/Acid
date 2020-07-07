#pragma once

#include <cmath>

#include "Maths/Time.hpp"

namespace acid {
/**
 * @brief Represents a driver that changes over time.
 * @tparam T The type to be driven.
 */
template<typename T>
class UiDriver {
public:
	/**
	 * Creates a new driver with a length.
	 * @param length The drivers length.
	 */
	explicit UiDriver(const Time &length) :
		length(length) {
	}

	virtual ~UiDriver() = default;

	/**
	 * Updates the driver with the passed time.
	 * @param delta The time between the last update.
	 * @return If the driver has not completed.
	 */
	bool Update(const Time &delta) {
		actualTime += delta;
		currentTime += delta;
		if (repeat)
			currentTime = Time::Seconds(std::fmod(currentTime.AsSeconds(), length.AsSeconds()));
		auto factor = static_cast<float>(currentTime / length);
		factor = std::clamp(factor, 0.0f, 1.0f);
		current = Calculate(factor);
		return factor != 1.0f && !repeat;
	}

	/**
	 * Gets the current driver value.
	 * @return The current value.
	 */
	T Get() const { return current; }

	/**
	 * Gets the length.
	 * @return The length.
	 */
	const Time &GetLength() const { return length; }
	/**
	 * Sets the length.
	 * @param length The new length.
	 */
	void SetLength(const Time &length) { this->length = length; }

protected:
	/**
	 * Calculates the new value.
	 * @param factor The time into the drivers life.
	 * @return The calculated value.
	 */
	virtual T Calculate(float factor) = 0;

	/// The most recent value calculation.
	T current{};
	
	Time length;
	bool repeat = true;
	Time actualTime;
	Time currentTime;
};
}
