#include "JoystickHatInput.hpp"

namespace acid {
JoystickHatInput::JoystickHatInput(JoystickPort port, JoystickHat hat, const bitmask::bitmask<JoystickHatValue> &hatFlags) :
	hat(hat),
	hatFlags(hatFlags) {
	SetPort(port);
}

InputAxis::ArgumentDescription JoystickHatInput::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"inverted", "bool", "If the down reading will be inverted"},
		{"axis", "axis", "The axis to sample"},
		{"min", "float", "Lower axis value bound"},
		{"max", "float", "Upper axis value bound"}
	};
}

float JoystickHatInput::GetAmount() const {
	auto hatValue = joystick->GetHat(hat);
	float value = 0.0f;
	if (hatValue & JoystickHatValue::Up) {
		if (hatValue & JoystickHatValue::Right)
			value = 0.125f;
		else if (hatValue & JoystickHatValue::Left)
			value = 0.875f;
		else
			value = 1.0f;
	}
	else if (hatValue & JoystickHatValue::Down) {
		if (hatValue & JoystickHatValue::Right)
			value = 0.375f;
		else if (hatValue & JoystickHatValue::Left)
			value = 0.625f;
		else
			value = 0.5f;
	}
	else if (hatValue & JoystickHatValue::Right)
		value = 0.25f;
	else if (hatValue & JoystickHatValue::Left)
		value = 0.75f;
	return scale * value + offset;
}

bool JoystickHatInput::IsDown() const {
	return (joystick->GetHat(hat) & hatFlags).value ^ static_cast<uint8_t>(inverted);
}

void JoystickHatInput::SetPort(JoystickPort port) {
	joystick = Joysticks::Get()->GetJoystick(port);
	disconnect_tracked_connections();
	joystick->OnHat().connect(static_cast<InputButton *>(this), [this](JoystickHat hat, bitmask::bitmask<JoystickHatValue> value) {
		if (this->hat == hat) {
			onAxis(GetAmount());
			auto isDown = IsDown();

			if (!lastDown && isDown) {
				lastDown = true;
				onButton(InputAction::Press, 0);
			} else if (lastDown && !isDown) {
				lastDown = false;
				onButton(InputAction::Release, 0);
			} else if (lastDown && isDown) {
				onButton(InputAction::Repeat, 0);
			}
		}
	});
}

const Node &operator>>(const Node &node, JoystickHatInput &input) {
	node["scale"].Get(input.scale);
	node["inverted"].Get(input.inverted);
	input.SetPort(node["port"].Get<JoystickPort>());
	node["hat"].Get(input.hat);
	node["hatFlags"].Get(input.hatFlags);
	return node;
}

Node &operator<<(Node &node, const JoystickHatInput &input) {
	node["scale"].Set(input.scale);
	node["inverted"].Set(input.inverted);
	node["port"].Set(input.GetPort());
	node["hat"].Set(input.hat);
	node["hatFlags"].Set(input.hatFlags);
	return node;
}
}
