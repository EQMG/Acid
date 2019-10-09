#include "ButtonJoystick.hpp"

namespace acid {
ButtonJoystick::ButtonJoystick(JoystickPort port, JoystickButton button) :
	m_port(port),
	m_button(button) {
	Joysticks::Get()->OnButton().Add([this](JoystickPort port, JoystickButton button, InputAction action) {
		if (port == m_port && button == m_button)
			m_onButton(action, 0);
	}, this);
}

bool ButtonJoystick::IsDown() const {
	return (Joysticks::Get()->GetButton(m_port, m_button) != InputAction::Release) ^ m_inverted;
}
}
