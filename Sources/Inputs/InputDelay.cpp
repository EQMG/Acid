#include "InputDelay.hpp"

namespace acid
{
InputDelay::InputDelay(const Time &delay, const Time &repeat) :
	m_elapsedDelay(delay),
	m_elapsedRepeat(repeat),
	m_delayOver(false)
{
}

void InputDelay::Update(const bool &keyIsDown)
{
	if (keyIsDown)
	{
		m_delayOver = m_elapsedDelay.GetElapsed() != 0;
	}
	else
	{
		m_delayOver = false;
		//m_elapsedDelay.SetStartTime(Time::Now());
		//m_elapsedRepeat.SetStartTime(Time::Now());
	}
}

bool InputDelay::CanInput()
{
	return m_delayOver && m_elapsedRepeat.GetElapsed() != 0;
}
}
