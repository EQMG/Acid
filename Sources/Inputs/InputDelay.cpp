#include "InputDelay.hpp"

namespace acid {
InputDelay::InputDelay(const Time &delay, const Time &repeat) :
	m_elapsedDelay(delay),
	m_elapsedRepeat(repeat) {
}

void InputDelay::Update(bool keyIsDown) {
	if (keyIsDown) {
		m_delayOver = m_elapsedDelay.GetElapsed() != 0;
	} else {
		m_delayOver = false;
		m_elapsedDelay.SetStartTime(0s);
		m_elapsedRepeat.SetStartTime(0s);
	}
}

bool InputDelay::CanInput() {
	return m_delayOver && m_elapsedRepeat.GetElapsed() != 0;
}
}
