#include "HatJoystick.hpp"

namespace acid {
HatJoystick::HatJoystick(JoystickPort port, JoystickHat hat, const BitMask<JoystickHatValue> &hatFlags) :
	port(port),
	hat(hat),
	hatFlags(hatFlags) {
	Joysticks::Get()->OnHat().Add([this](JoystickPort port, JoystickHat hat, BitMask<JoystickHatValue> value) {
		if (this->port == port && this->hat == hat) {
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
	}, static_cast<Button *>(this));
}

Axis::ArgumentDescription HatJoystick::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"inverted", "bool", "If the down reading will be inverted"},
		{"axis", "axis", "The axis to sample"},
		{"min", "float", "Lower axis value bound"},
		{"max", "float", "Upper axis value bound"}
	};
}

float HatJoystick::GetAmount() const {
	auto hatValue = Joysticks::Get()->GetHat(port, hat);
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
	return scale * value;
}

bool HatJoystick::IsDown() const {
	return (Joysticks::Get()->GetHat(port, hat) & hatFlags) ^ inverted;
}

const Node &operator>>(const Node &node, HatJoystick &hatJoystick) {
	node["scale"].Get(hatJoystick.scale);
	node["inverted"].Get(hatJoystick.inverted);
	node["port"].Get(hatJoystick.port);
	node["hat"].Get(hatJoystick.hat);
	node["hatFlags"].Get(hatJoystick.hatFlags);
	return node;
}

Node &operator<<(Node &node, const HatJoystick &hatJoystick) {
	node["scale"].Set(hatJoystick.scale);
	node["inverted"].Set(hatJoystick.inverted);
	node["port"].Set(hatJoystick.port);
	node["hat"].Set(hatJoystick.hat);
	node["hatFlags"].Set(hatJoystick.hatFlags);
	return node;
}
}
