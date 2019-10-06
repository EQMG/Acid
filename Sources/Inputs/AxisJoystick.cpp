#include "AxisJoystick.hpp"

namespace acid {
AxisJoystick::AxisJoystick(JoystickPort port, JoystickAxis axis) :
	m_port(port),
	m_axis(axis) {
	Joysticks::Get()->OnAxis().Add([this](JoystickPort port, JoystickAxis axis, float value) {
		if (port == m_port && axis == m_axis) {
			m_onAxis(GetAmount());
		}
	}, this);
}

float AxisJoystick::GetAmount() const {
	return m_scale * Joysticks::Get()->GetAxis(m_port, m_axis);
}

bool AxisJoystick::IsConnected() const {
	return Joysticks::Get()->IsConnected(m_port);
}
}
