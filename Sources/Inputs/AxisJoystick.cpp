#include "AxisJoystick.hpp"

namespace acid {
AxisJoystick::AxisJoystick(JoystickPort port, JoystickAxis axis, bool inverted) :
	m_port(port),
	m_axis(axis),
	m_inverted(inverted) {
	Joysticks::Get()->OnAxis().Add([this](JoystickPort port, JoystickAxis axis, float value) {
		if (port == m_port && axis == m_axis) {
			m_onAxis(value);
		}
	}, this);
}

float AxisJoystick::GetAmount() const {
	return Joysticks::Get()->GetAxis(m_port, m_axis) * (m_inverted ? -1.0f : 1.0f);
}

bool AxisJoystick::IsConnected() const {
	return Joysticks::Get()->IsConnected(m_port);
}
}
