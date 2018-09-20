#include "DriverLinear.hpp"

namespace acid
{
	DriverLinear::DriverLinear(const float &start, const float &end, const float &length) :
		IDriver(length),
		m_start(start),
		m_end(end)
	{
	}

	float DriverLinear::Calculate(const float &time)
	{
		return m_start + time * (m_end - m_start);
	}
}
