#include "DriverBounce.hpp"

namespace acid
{
	DriverBounce::DriverBounce(const float &start, const float &end, const Time &length) :
		IDriver(length),
		m_start(start),
		m_end(end)
	{
	}

	float DriverBounce::Calculate(const float &factor)
	{
		float value = 0.5f + std::sin(PI * 2.0f * factor) * 0.5f;

		if (m_actualTime > GetLength() / 2.0f)
		{
			value = 0.0f;
		}

		return m_start + value * (m_end - m_start);
	}
}
