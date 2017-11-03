#include "DriverSlide.hpp"

namespace Flounder
{
	DriverSlide::DriverSlide(const float &start, const float &end, const float &length) :
		IDriver(length),
		m_start(start),
		m_end(end),
		m_max(0.0f),
		m_reachedTarget(false)
	{
	}

	DriverSlide::~DriverSlide()
	{
	}

	float DriverSlide::Calculate(const float &time)
	{
		if (!m_reachedTarget && time >= m_max)
		{
			m_max = time;
		//	return m_start + time * (m_end - m_start);
			return Maths::CosInterpolate(m_start, m_end, time);
		}

		m_reachedTarget = true;
		return m_start + (m_end - m_start);
	}
}
