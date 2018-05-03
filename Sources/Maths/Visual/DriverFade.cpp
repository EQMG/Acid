#include "DriverFade.hpp"

namespace fl
{
	DriverFade::DriverFade(const float &start, const float &end, const float &peak, const float &length) :
		IDriver(length),
		m_start(start),
		m_end(end),
		m_peak(peak)
	{
	}

	DriverFade::~DriverFade()
	{
	}

	float DriverFade::Calculate(const float &time)
	{
		if (time < m_start)
		{
			return time / m_start * m_peak;
		}

		if (time > m_end)
		{
			return (1.0f - (time - m_end) / (1.0f - m_end)) * m_peak;
		}

		return m_peak;
	}
}
