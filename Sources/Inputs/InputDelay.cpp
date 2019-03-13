#include "InputDelay.hpp"

namespace acid
{
InputDelay::InputDelay(const Time& delay, const Time& repeat) : m_timerDelay(delay), m_timerRepeat(repeat), m_delayOver(false) {}

void InputDelay::Update(const bool& keyIsDown)
{
	if(keyIsDown)
		{
			m_delayOver = m_timerDelay.IsPassedTime();
		}
	else
		{
			m_delayOver = false;
			m_timerDelay.ResetStartTime();
			m_timerRepeat.ResetStartTime();
		}
}

bool InputDelay::CanInput()
{
	if(m_delayOver && m_timerRepeat.IsPassedTime())
		{
			m_timerRepeat.ResetStartTime();
			return true;
		}

	return false;
}
}
