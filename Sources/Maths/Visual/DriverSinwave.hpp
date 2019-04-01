#pragma once

#include "Maths/Maths.hpp"
#include "IDriver.hpp"

namespace acid
{
/**
 * @brief A driver that uses a sine wave.
 * @tparam T The type to be driven.
 **/
template<typename T>
class DriverSinwave :
	public IDriver<T>
{
public:
	/**
	 * Creates a new sine wave driver.
	 * @param min The min value. 
	 * @param max The max value. 
	 * @param length The length between two waves. 
	 **/
	DriverSinwave(const T &min, const T &max, const Time &length) :
		IDriver<T>(length),
		m_min(min),
		m_max(max)
	{
	}

	/**
	 * Gets the min value.
	 * @return The min value. 
	 **/
	const T &GetMin() const { return m_min; }

	/**
	 * Sets the min value.
	 * @param min The new min value. 
	 **/
	void SetMin(const T &min) { m_min = min; }

	/**
	 * Gets the max value.
	 * @return The max value. 
	 **/
	const T &GetMax() const { return m_max; }

	/**
	 * Sets the max value.
	 * @param max The new max value. 
	 **/
	void SetMax(const T &max) { m_max = max; }

protected:
	T Calculate(const float &factor) override
	{
		float value = 0.5f + std::sin(2.0f * Maths::Pi * factor) * 0.5f;
		return m_min + value * (m_max - m_min);
	}

private:
	T m_min;
	T m_max;
};
}
