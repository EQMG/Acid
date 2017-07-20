#include "driverbounce.h"

namespace flounder 
{
	driverbounce::driverbounce(const float &start, const float &end, const float &length)
		: idriver(length)
	{
		m_start = start;
		m_amplitude = end - start;
		m_length = length;
	}

	driverbounce::~driverbounce()
	{
	}

	float driverbounce::calculate(const float &time)
	{
		float value = 0.5f + (float) sin(PI * 2.0 * time) * 0.5f;

		if (getActualTime() > m_length / 2.0f)
		{
			value = 0.0f;
		}

		return m_start + value * m_amplitude;
	}
}
