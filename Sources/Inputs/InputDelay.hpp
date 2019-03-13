#pragma once

#include "Maths/Timer.hpp"

namespace acid
{
class ACID_EXPORT InputDelay
{
  public:
	explicit InputDelay(const Time& delay = Time::Seconds(0.2f), const Time& repeat = Time::Seconds(0.06f));

	void Update(const bool& keyIsDown);

	bool CanInput();

	const Time& GetDelay() const
	{
		return m_timerDelay.GetInterval();
	}

	void SetDelay(const Time& delay)
	{
		m_timerDelay.SetInterval(delay);
	}

	const Time& GetRepeat() const
	{
		return m_timerRepeat.GetInterval();
	}

	void SetRepeat(const Time& repeat)
	{
		m_timerRepeat.SetInterval(repeat);
	}

  private:
	Timer m_timerDelay;
	Timer m_timerRepeat;
	bool m_delayOver;
};
}
