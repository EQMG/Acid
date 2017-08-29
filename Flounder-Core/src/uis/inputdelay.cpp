#include "inputdelay.hpp"

namespace flounder
{
	inputdelay::inputdelay()
	{
		m_timerDelay = new timer(0.4f);
		m_timerRepeat = new timer(0.1f);
		m_delayOver = false;
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
			m_delayOver = m_timerDelay->isPassedTime();
		}
		else
		{
			m_delayOver = false;
			m_timerDelay->resetStartTime();
			m_timerRepeat->resetStartTime();
		}
	}

	bool inputdelay::canInput()
	{
		if (m_delayOver && m_timerRepeat->isPassedTime())
		{
			m_timerRepeat->resetStartTime();
			return true;
		}

		return false;
	}
}
