#include "JoystickInputAxis.hpp"

namespace acid {
JoystickInputAxis::JoystickInputAxis(JoystickPort port, JoystickAxis axis) :
	axis(axis) {
	SetPort(port);
}

float JoystickInputAxis::GetAmount() const {
	return scale * joystick->GetAxis(axis) + offset;
}

void JoystickInputAxis::SetPort(JoystickPort port) {
	joystick = Joysticks::Get()->GetJoystick(port);
	disconnect_tracked_connections();
	joystick->OnAxis().connect(this, [this](JoystickAxis axis, float value) {
		if (this->axis == axis)
			onAxis(GetAmount());
	});
}

InputAxis::ArgumentDescription JoystickInputAxis::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"port", "int", "The joystick port name"},
		{"axis", "int", "The axis on the joystick being checked"}
	};
}

const Node &operator>>(const Node &node, JoystickInputAxis &inputAxis) {
	node["scale"].Get(inputAxis.scale);
	inputAxis.SetPort(node["port"].Get<JoystickPort>());
	node["axis"].Get(inputAxis.axis);
	return node;
}

Node &operator<<(Node &node, const JoystickInputAxis &inputAxis) {
	node["scale"].Set(inputAxis.scale);
	node["port"].Set(inputAxis.GetPort());
	node["axis"].Set(inputAxis.axis);
	return node;
}
}
