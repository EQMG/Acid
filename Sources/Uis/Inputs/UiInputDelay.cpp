﻿#include "UiInputDelay.hpp"

namespace acid
{
	const Time UiInputDelay::DELAY_TIME = Time::Seconds(0.4f);
	const Time UiInputDelay::REPEAT_TIME = Time::Seconds(0.1f);

	UiInputDelay::UiInputDelay() :
		m_timerDelay(Timer(DELAY_TIME)),
		m_timerRepeat(Timer(REPEAT_TIME)),
		m_delayOver(false)
	{
	}

	void UiInputDelay::Update(const bool &keyIsDown)
	{
		if (keyIsDown)
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

	bool UiInputDelay::CanInput()
	{
		if (m_delayOver && m_timerRepeat.IsPassedTime())
		{
			m_timerRepeat.ResetStartTime();
			return true;
		}

		return false;
	}
}
