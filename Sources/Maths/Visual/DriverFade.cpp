#include "DriverFade.hpp"

namespace acid
{
	DriverFade::DriverFade(const float &start, const float &end, const float &peak, const Time &length) :
		IDriver(length),
		m_start(start),
		m_end(end),
		m_peak(peak)
	{
	}

	float DriverFade::Calculate(const float &factor)
	{
		if (factor < m_start)
		{
			return factor / m_start * m_peak;
		}

		if (factor > m_end)
		{
			return (1.0f - (factor - m_end) / (1.0f - m_end)) * m_peak;
		}

		return m_peak;
	}
}
