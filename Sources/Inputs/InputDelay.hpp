#pragma once

#include "Engine/Engine.hpp"

namespace acid {
class ACID_EXPORT InputDelay {
public:
	explicit InputDelay(const Time &delay = 0.06s, const Time & repeat = Time::Seconds(0.06f));

	void Update(bool keyIsDown);

	bool CanInput();

	const Time &GetDelay() const { return m_elapsedDelay.GetInterval(); }
	void SetDelay(const Time &delay) { m_elapsedDelay.SetInterval(delay); }

	const Time &GetRepeat() const { return m_elapsedRepeat.GetInterval(); }
	void SetRepeat(const Time &repeat) { m_elapsedRepeat.SetInterval(repeat); }

private:
	ElapsedTime m_elapsedDelay;
	ElapsedTime m_elapsedRepeat;
	bool m_delayOver = false;
};
}
