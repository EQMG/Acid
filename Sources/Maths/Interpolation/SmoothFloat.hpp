#pragma once

#include "Maths/Time.hpp"

namespace acid {
/**
 * @brief Class that smoothly increases its value.
 * @tparam T The type to be driven.
 */
template<typename T>
class SmoothFloat {
public:
	/**
	 * Creates a new smooth float.
	 * @param initialValue The initial value.
	 * @param agility The agility for increasing actual.
	 */
	SmoothFloat(const T &initialValue, float agility) :
		m_agility(agility),
		m_target(initialValue),
		m_actual(initialValue) {
	}

	/**
	 * Updates the driver with the passed time.
	 * @param delta The time between the last update.
	 */
	void Update(const Time &delta) {
		auto offset = m_target - m_actual;
		auto change = offset * delta.AsSeconds() * m_agility;
		m_actual += change;
	}

	/**
	 * Gets the smooth floats current target.
	 * @return The target.
	 */
	const T &GetTarget() const { return m_target; }

	/**
	 * Sets the target for the smooth float.
	 * @param target The new target.
	 */
	void SetTarget(const T &target) { m_target = target; }

	/**
	 * Increases the smooth floats target.
	 * @param increase How much to increase the target by.
	 */
	void IncreaseTarget(const T &increase) { m_target += increase; }

	/**
	 * Gets the currently calculated value.
	 * @return The calculated value.
	 */
	float Get() const { return m_actual; }

private:
	float m_agility;
	T m_target;
	T m_actual;
};
}
