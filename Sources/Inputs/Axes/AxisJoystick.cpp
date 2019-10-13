#include "AxisJoystick.hpp"

namespace acid {
bool AxisJoystick::registered = Register("axisJoystick");

AxisJoystick::AxisJoystick(JoystickPort port, JoystickAxis axis) :
	m_port(port),
	m_axis(axis) {
	Joysticks::Get()->OnAxis().Add([this](JoystickPort port, JoystickAxis axis, float value) {
		if (port == m_port && axis == m_axis)
			m_onAxis(GetAmount());
	}, this);
}

float AxisJoystick::GetAmount() const {
	return m_scale * Joysticks::Get()->GetAxis(m_port, m_axis);
}

Axis::ArgumentDescription AxisJoystick::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"port", "int", "The joystick port name"},
		{"axis", "int", "The axis on the joystick being checked"}
	};
}

bool AxisJoystick::IsConnected() const {
	return Joysticks::Get()->IsConnected(m_port);
}

const Node &operator>>(const Node &node, AxisJoystick &axisJoystick) {
	node["scale"].Get(axisJoystick.m_scale);
	node["port"].Get(axisJoystick.m_port);
	node["axis"].Get(axisJoystick.m_axis);
	return node;
}

Node &operator<<(Node &node, const AxisJoystick &axisJoystick) {
	node["scale"].Set(axisJoystick.m_scale);
	node["port"].Set(axisJoystick.m_port);
	node["axis"].Set(axisJoystick.m_axis);
	return node;
}
}
