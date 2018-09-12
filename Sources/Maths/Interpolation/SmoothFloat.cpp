#include "SmoothFloat.hpp"

namespace acid
{
	SmoothFloat::SmoothFloat(const float &initialValue, const float &agility) :
		m_agility(agility),
		m_target(initialValue),
		m_actual(initialValue)
	{
	}

	void SmoothFloat::Update(const float &delta)
	{
		float offset = m_target - m_actual;
		float change = offset * delta * m_agility;
		m_actual += change;
	}
}
