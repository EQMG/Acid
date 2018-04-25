#include "DriverLinear.hpp"

namespace fl
{
	DriverLinear::DriverLinear(const float &startValue, const float &endValue, const float &length) :
		IDriver(length),
		m_startValue(startValue),
		m_difference(endValue - startValue)
	{
	}

	DriverLinear::~DriverLinear()
	{
	}

	float DriverLinear::Calculate(const float &time)
	{
		return m_startValue + time * m_difference;
	}
}
