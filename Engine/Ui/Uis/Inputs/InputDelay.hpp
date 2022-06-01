#pragma once

#include "Maths/ElapsedTime.hpp"
#include "Ui/Export.hpp"

namespace acid {
class ACID_UI_EXPORT InputDelay {
public:
	explicit InputDelay(const Time &delay = 0.06s, const Time &repeat = Time::Seconds(0.06f));

	void Update(bool keyIsDown);

	bool CanInput();

	const Time &GetDelay() const { return elapsedDelay.GetInterval(); }
	void SetDelay(const Time &delay) { elapsedDelay.SetInterval(delay); }

	const Time &GetRepeat() const { return elapsedRepeat.GetInterval(); }
	void SetRepeat(const Time &repeat) { elapsedRepeat.SetInterval(repeat); }

private:
	ElapsedTime elapsedDelay;
	ElapsedTime elapsedRepeat;
	bool delayOver = false;
};
}
