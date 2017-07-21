#include "driversinwave.h"

namespace flounder
{
	driversinwave::driversinwave(const float &min, const float &max, const float &length)
		: idriver(length)
	{
		m_min = min;
		m_amplitude = max - min;
	}

	driversinwave::~driversinwave()
	{
	}

	float driversinwave::calculate(const float &time)
	{
		float value = 0.5f + (float) sin(2.0 * PI * time) * 0.5f;
		return m_min + value * m_amplitude;
	}
}
