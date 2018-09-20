#include "DriverBounce.hpp"

namespace acid
{
	DriverBounce::DriverBounce(const float &start, const float &end, const float &length) :
		IDriver(length),
		m_start(start),
		m_end(end)
	{
	}

	float DriverBounce::Calculate(const float &time)
	{
		float value = 0.5f + std::sin(PI * 2.0f * time) * 0.5f;

		if (GetActualTime() > GetLength() / 2.0f)
		{
			value = 0.0f;
		}

		return m_start + value * (m_end - m_start);
	}
}
