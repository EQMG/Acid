#include "JoystickInputAxis.hpp"

namespace acid {
JoystickInputAxis::JoystickInputAxis(JoystickPort port, JoystickAxis axis) :
	port(port),
	axis(axis) {
	Joysticks::Get()->OnAxis().Add([this](JoystickPort port, JoystickAxis axis, float value) {
		if (this->port == port && this->axis == axis)
			onAxis(GetAmount());
	}, this);
}

float JoystickInputAxis::GetAmount() const {
	return scale * Joysticks::Get()->GetAxis(port, axis);
}

InputAxis::ArgumentDescription JoystickInputAxis::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"port", "int", "The joystick port name"},
		{"axis", "int", "The axis on the joystick being checked"}
	};
}

bool JoystickInputAxis::IsConnected() const {
	return Joysticks::Get()->IsConnected(port);
}

const Node &operator>>(const Node &node, JoystickInputAxis &inputAxis) {
	node["scale"].Get(inputAxis.scale);
	node["port"].Get(inputAxis.port);
	node["axis"].Get(inputAxis.axis);
	return node;
}

Node &operator<<(Node &node, const JoystickInputAxis &inputAxis) {
	node["scale"].Set(inputAxis.scale);
	node["port"].Set(inputAxis.port);
	node["axis"].Set(inputAxis.axis);
	return node;
}
}
