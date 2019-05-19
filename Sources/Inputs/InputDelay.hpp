#pragma once

#include "Engine/Engine.hpp"

namespace acid
{
class ACID_EXPORT InputDelay
{
public:
	explicit InputDelay(const Time &delay = 0.2s, const Time &repeat = Time::Seconds(0.06f));

	void Update(const bool &keyIsDown);

	bool CanInput();

	const Time &GetDelay() const { return m_timerDelay.GetInterval(); }

	void SetDelay(const Time &delay) { m_timerDelay.SetInterval(delay); }

	const Time &GetRepeat() const { return m_timerRepeat.GetInterval(); }

	void SetRepeat(const Time &repeat) { m_timerRepeat.SetInterval(repeat); }

private:
	DeltaTimer m_timerDelay;
	DeltaTimer m_timerRepeat;
	bool m_delayOver;
};
}
