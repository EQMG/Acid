#include "DriverLinear.hpp"

namespace acid
{
	DriverLinear::DriverLinear(const float &start, const float &end, const Time &length) :
		IDriver(length),
		m_start(start),
		m_end(end)
	{
	}

	float DriverLinear::Calculate(const float &factor)
	{
		return m_start + factor * (m_end - m_start);
	}
}
