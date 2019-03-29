#pragma once

#include "IDriver.hpp"

namespace acid
{
/// <summary>
/// A driver that has a constant value.
/// </summary>
template<typename T> class DriverConstant :
	public IDriver<T>
{
public:
	/// <summary>
	/// Creates a new constant driver.
	/// </summary>
	/// <param name="constant"> The constant value. </param>
	explicit DriverConstant(const T &constant) :
		IDriver<T>(Time::Max),
		m_constant(constant)
	{
	}

	/// <summary>
	/// Gets the constant.
	/// </summary>
	/// <returns> The constant. </returns>
	const T &GetConstant() const { return m_constant; }

	/// <summary>
	/// Sets the constant.
	/// </summary>
	/// <param name="constant"> The new constant. </param>
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
