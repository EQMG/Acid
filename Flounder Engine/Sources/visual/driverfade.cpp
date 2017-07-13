#include "driverfade.h"

namespace flounder {
	driverfade::driverfade(const float &start, const float &end, const float &peak, const float &length)
		: idriver(length)
	{
		m_start = start;
		m_end = end;
		m_peak = peak;
	}

	driverfade::~driverfade()
	{
	}

	float driverfade::calculate(const float &time)
	{
		if (time < m_start)
		{
			return time / m_start * m_peak;
		}
		else if (time > m_end)
		{
			return (1.0f - (time - m_end) / (1.0f - m_end)) * m_peak;
		}
		else
		{
			return m_peak;
		}
	}
}
