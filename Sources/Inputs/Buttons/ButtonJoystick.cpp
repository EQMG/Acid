#include "ButtonJoystick.hpp"

namespace acid {
bool ButtonJoystick::registered = Register("buttonJoystick");

ButtonJoystick::ButtonJoystick(JoystickPort port, JoystickButton button) :
	m_port(port),
	m_button(button) {
	Joysticks::Get()->OnButton().Add([this](JoystickPort port, JoystickButton button, InputAction action) {
		if (port == m_port && button == m_button) {
			m_onButton(action, 0);
		}
	}, this);
}

bool ButtonJoystick::IsDown() const {
	return (Joysticks::Get()->GetButton(m_port, m_button) != InputAction::Release) ^ m_inverted;
}

Axis::ArgumentDescription ButtonJoystick::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"port", "int", "The joystick port name"},
		{"axis", "bool", "The button on the joystick being checked"}
	};
}

const Node &operator>>(const Node &node, ButtonJoystick &buttonJoystick) {
	node["inverted"].Get(buttonJoystick.m_inverted);
	node["port"].Get(buttonJoystick.m_port);
	node["button"].Get(buttonJoystick.m_button);
	return node;
}

Node &operator<<(Node &node, const ButtonJoystick &buttonJoystick) {
	node["inverted"].Set(buttonJoystick.m_inverted);
	node["port"].Set(buttonJoystick.m_port);
	node["button"].Set(buttonJoystick.m_button);
	return node;
}
}
