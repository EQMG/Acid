#include "inputdelay.hpp"

namespace Flounder
{
	inputdelay::inputdelay() :
		m_timerDelay(new Timer(0.4f)),
		m_timerRepeat(new Timer(0.1f)),
		m_delayOver(false)
	{
	}

	inputdelay::~inputdelay()
	{
		delete m_timerDelay;
		delete m_timerRepeat;
	}

	void inputdelay::update(const bool &keyIsDown)
	{
		if (keyIsDown)
		{
			m_delayOver = m_timerDelay->IsPassedTime();
		}
		else
		{
			m_delayOver = false;
			m_timerDelay->ResetStartTime();
			m_timerRepeat->ResetStartTime();
		}
	}

	bool inputdelay::canInput()
	{
		if (m_delayOver && m_timerRepeat->IsPassedTime())
		{
			m_timerRepeat->ResetStartTime();
			return true;
		}

		return false;
	}
}
