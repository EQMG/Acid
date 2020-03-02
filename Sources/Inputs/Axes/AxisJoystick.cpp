#include "AxisJoystick.hpp"

namespace acid {
AxisJoystick::AxisJoystick(JoystickPort port, JoystickAxis axis) :
	port(port),
	axis(axis) {
	Joysticks::Get()->OnAxis().Add([this](JoystickPort port, JoystickAxis axis, float value) {
		if (this->port == port && this->axis == axis)
			onAxis(GetAmount());
	}, this);
}

float AxisJoystick::GetAmount() const {
	return scale * Joysticks::Get()->GetAxis(port, axis);
}

Axis::ArgumentDescription AxisJoystick::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"port", "int", "The joystick port name"},
		{"axis", "int", "The axis on the joystick being checked"}
	};
}

bool AxisJoystick::IsConnected() const {
	return Joysticks::Get()->IsConnected(port);
}

const Node &operator>>(const Node &node, AxisJoystick &axisJoystick) {
	node["scale"].Get(axisJoystick.scale);
	node["port"].Get(axisJoystick.port);
	node["axis"].Get(axisJoystick.axis);
	return node;
}

Node &operator<<(Node &node, const AxisJoystick &axisJoystick) {
	node["scale"].Set(axisJoystick.scale);
	node["port"].Set(axisJoystick.port);
	node["axis"].Set(axisJoystick.axis);
	return node;
}
}
