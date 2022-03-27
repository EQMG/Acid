#include "JoystickInputButton.hpp"

namespace acid {
JoystickInputButton::JoystickInputButton(JoystickPort port, JoystickButton button) :
	button(button) {
	SetPort(port);
}

bool JoystickInputButton::IsDown() const {
	return (joystick->GetButton(button) != InputAction::Release) ^ inverted;
}

void JoystickInputButton::SetPort(JoystickPort port) {
	joystick = Joysticks::Get()->GetJoystick(port);
	disconnect_tracked_connections();
	joystick->OnButton().connect(this, [this](JoystickButton button, InputAction action) {
		if (this->button == button)
			onButton(action, 0);
	});
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
	inputButton.SetPort(node["port"].Get<JoystickPort>());
	node["button"].Get(inputButton.button);
	return node;
}

Node &operator<<(Node &node, const JoystickInputButton &inputButton) {
	node["inverted"].Set(inputButton.inverted);
	node["port"].Set(inputButton.GetPort());
	node["button"].Set(inputButton.button);
	return node;
}
}
