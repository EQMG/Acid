#pragma once

#include "Maths/Time.hpp"

namespace acid
{
/**
 * @brief Represents a driver that changes over time.
 * @tparam T The type to be driven.
 **/
template<typename T>
class Driver
{
public:
	/**
	 * Creates a new driver with a length.
	 * @param length The drivers length. 
	 **/
	explicit Driver(const Time &length) :
		m_length{length}
	{
	}

	virtual ~Driver() = default;

	/**
	 * Updates the driver with the passed time.
	 * @param delta The time between the last update. 
	 * @return The calculated value. 
	 **/

	T Update(const Time &delta)
	{
		m_actualTime += delta;
		m_currentTime += delta;
		m_currentTime = Time::Seconds(std::fmod(m_currentTime.AsSeconds(), m_length.AsSeconds()));
		auto time = static_cast<float>(m_currentTime / m_length);
		return Calculate(time);
	}

	/**
	 * Gets the length.
	 * @return The length. 
	 **/
	const Time &GetLength() const { return m_length; }

	/**
	 * Sets the length.
	 * @param length The new length. 
	 **/
	void SetLength(const Time &length) { m_length = length; }

protected:
	/**
	 * Calculates the new value.
	 * @param factor The time into the drivers life.
	 * @return The calculated value. 
	 **/
	virtual T Calculate(const float &factor) = 0;

	Time m_length;
	Time m_actualTime;
	Time m_currentTime;
};
}
