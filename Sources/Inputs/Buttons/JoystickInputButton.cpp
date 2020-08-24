#include "JoystickInputButton.hpp"

namespace acid {
const bool JoystickInputButton::Registered = Register("joystick");

JoystickInputButton::JoystickInputButton(JoystickPort port, JoystickButton button) :
	port(port),
	button(button) {
	Joysticks::Get()->OnButton().connect(this, [this](JoystickPort port, JoystickButton button, InputAction action) {
		if (this->port == port && this->button == button) {
			onButton(action, 0);
		}
	});
}

bool JoystickInputButton::IsDown() const {
	return (Joysticks::Get()->GetButton(port, button) != InputAction::Release) ^ inverted;
}

InputAxis::ArgumentDescription JoystickInputButton::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"port", "int", "The joystick port name"},
		{"axis", "bool", "The button on the joystick being checked"}
	};
}

const Node &operator>>(const Node &node, JoystickInputButton &inputButton) {
	node["inverted"].Get(inputButton.inverted);
	node["port"].Get(inputButton.port);
	node["button"].Get(inputButton.button);
	return node;
}

Node &operator<<(Node &node, const JoystickInputButton &inputButton) {
	node["inverted"].Set(inputButton.inverted);
	node["port"].Set(inputButton.port);
	node["button"].Set(inputButton.button);
	return node;
}
}
