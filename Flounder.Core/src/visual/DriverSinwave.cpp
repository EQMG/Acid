#include "DriverSinwave.hpp"

namespace Flounder
{
	DriverSinwave::DriverSinwave(const float &min, const float &max, const float &length) :
		IDriver(length),
		m_min(min),
		m_amplitude(max - min)
	{
	}

	DriverSinwave::~DriverSinwave()
	{
	}

	float DriverSinwave::Calculate(const float &time)
	{
		float value = 0.5f + sin(2.0f * PI * time) * 0.5f;
		return m_min + value * m_amplitude;
	}
}
