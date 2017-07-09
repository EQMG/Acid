#include "valuedriver.h"

namespace flounder {
	valuedriver::valuedriver(const float length)
	{
		m_currentTime = 0.0f;
		m_length = length;
	}

	valuedriver::~valuedriver()
	{
	}

	float valuedriver::update(float delta)
	{
		m_currentTime += delta;
		m_currentTime %= m_length;
		float time = m_currentTime / m_length;
		return calculateValue(time);
	}
}
