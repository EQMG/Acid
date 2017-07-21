#include "smoothfloat.h"

namespace flounder
{
	smoothfloat::smoothfloat(const float &initialValue, const float &agility)
	{
		m_target = initialValue;
		m_actual = initialValue;
		m_agility = agility;
	}

	smoothfloat::~smoothfloat()
	{
	}

	void smoothfloat::update(const float &delta)
	{
		float offset = m_target - m_actual;
		float change = offset * delta * m_agility;
		m_actual += change;
	}
}
