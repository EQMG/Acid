#include "driverslide.h"

namespace flounder
{
	driverslide::driverslide(const float &start, const float &end, const float &length)
		: idriver(length)
	{
		m_start = start;
		m_end = end;
		m_max = 0.0f;
		m_reachedTarget = false;
	}

	driverslide::~driverslide()
	{
	}

	float driverslide::calculate(const float &time)
	{
		if (!m_reachedTarget && time >= m_max)
		{
			m_max = time;
			return maths::cosInterpolate(m_start, m_end, time);
		}
		else
		{
			m_reachedTarget = true;
			return m_start + (m_end - m_start);
		}
	}
}
