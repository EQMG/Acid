#pragma once

#include "Driver.hpp"

namespace acid
{
/**
 * @brief A driver that has a constant value.
 * @tparam T The type to be driven.
 **/

template<typename T>
class DriverConstant :
	public Driver<T>
{
public:
	/**
	 * Creates a new constant driver.
	 * @param constant The constant value. 
	 **/
	explicit DriverConstant(const T &constant) :
		Driver<T>(Time::Max),
		m_constant(constant)
	{
	}

	/**
	 * Gets the constant.
	 * @return The constant. 
	 **/
	const T &GetConstant() const { return m_constant; }

	/**
	 * Sets the constant.
	 * @param constant The new constant. 
	 **/
	void SetConstant(const T &constant) { m_constant = constant; };
protected:
	T Calculate(const float &factor) override
	{
		return m_constant;
	}

private:
	T m_constant;
};
}
