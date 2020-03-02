#include "ButtonJoystick.hpp"

namespace acid {
ButtonJoystick::ButtonJoystick(JoystickPort port, JoystickButton button) :
	port(port),
	button(button) {
	Joysticks::Get()->OnButton().Add([this](JoystickPort port, JoystickButton button, InputAction action) {
		if (this->port == port && this->button == button) {
			onButton(action, 0);
		}
	}, this);
}

bool ButtonJoystick::IsDown() const {
	return (Joysticks::Get()->GetButton(port, button) != InputAction::Release) ^ inverted;
}

Axis::ArgumentDescription ButtonJoystick::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"port", "int", "The joystick port name"},
		{"axis", "bool", "The button on the joystick being checked"}
	};
}

const Node &operator>>(const Node &node, ButtonJoystick &buttonJoystick) {
	node["inverted"].Get(buttonJoystick.inverted);
	node["port"].Get(buttonJoystick.port);
	node["button"].Get(buttonJoystick.button);
	return node;
}

Node &operator<<(Node &node, const ButtonJoystick &buttonJoystick) {
	node["inverted"].Set(buttonJoystick.inverted);
	node["port"].Set(buttonJoystick.port);
	node["button"].Set(buttonJoystick.button);
	return node;
}
}
