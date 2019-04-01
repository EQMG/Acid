#pragma once

#include "Maths/Time.hpp"

namespace acid
{
/**
 * @brief Represents a driver that changes over time.
 * @tparam T The type to be driven.
 **/
template<typename T>
class IDriver
{
public:
	/**
	 * Creates a new driver with a length.
	 * @param length The drivers length. 
	 **/
	explicit IDriver(const Time &length) :
		m_length(length),
		m_actualTime(Time::Zero),
		m_currentTime(Time::Zero)
	{
	}

	virtual ~IDriver() = default;

	/**
	 * Updates the driver with the passed time.
	 * @param delta The time between the last update. 
	 * @return The calculated value. 
	 **/
	T Update(const Time &delta)
	{
		m_actualTime += delta;
		m_currentTime += delta;
		m_currentTime = m_currentTime % m_length;
		auto time = m_currentTime / m_length;
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
	 * @param time The time into the drivers life.
	 * </param>
	 * @return The calculated value. 
	 **/
	virtual T Calculate(const float &factor) = 0;

	Time m_length;
	Time m_actualTime;
	Time m_currentTime;
};
}
