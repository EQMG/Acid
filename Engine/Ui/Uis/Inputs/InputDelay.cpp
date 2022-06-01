#include "InputDelay.hpp"

namespace acid {
InputDelay::InputDelay(const Time &delay, const Time &repeat) :
	elapsedDelay(delay),
	elapsedRepeat(repeat) {
}

void InputDelay::Update(bool keyIsDown) {
	if (keyIsDown) {
		delayOver = elapsedDelay.GetElapsed() != 0;
	} else {
		delayOver = false;
		elapsedDelay.SetStartTime(0s);
		elapsedRepeat.SetStartTime(0s);
	}
}

bool InputDelay::CanInput() {
	return delayOver && elapsedRepeat.GetElapsed() != 0;
}
}
