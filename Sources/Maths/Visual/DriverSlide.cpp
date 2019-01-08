#include "DriverSlide.hpp"

namespace acid
{
	DriverSlide::DriverSlide(const float &start, const float &end, const Time &length) :
		IDriver(length),
		m_start(start),
		m_end(end)
	{
	}

	float DriverSlide::Calculate(const float &factor)
	{
		float realTime = std::min(m_actualTime, GetLength()) / GetLength();
		return m_start + realTime * (m_end - m_start);
	//	return Maths::CosInterpolate(m_start, m_end, realTime);
	}
}
