#include "DriverSinwave.hpp"

namespace acid
{
	DriverSinwave::DriverSinwave(const float &min, const float &max, const Time &length) :
		IDriver(length),
		m_min(min),
		m_max(max)
	{
	}

	float DriverSinwave::Calculate(const float &factor)
	{
		float value = 0.5f + std::sin(2.0f * Maths::Pi * factor) * 0.5f;
		return m_min + value * (m_max - m_min);
	}
}
