#include "driverlinear.hpp"

namespace flounder
{
	driverlinear::driverlinear(const float &startValue, const float &endValue, const float &length)
		: idriver(length)
	{
		m_startValue = startValue;
		m_difference = endValue - startValue;
	}

	driverlinear::~driverlinear()
	{
	}

	float driverlinear::calculate(const float &time)
	{
		return m_startValue + time * m_difference;
	}
}
