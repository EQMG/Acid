#include "AxisJoystick.hpp"

#include "Devices/Joysticks.hpp"

namespace acid
{
AxisJoystick::AxisJoystick(const uint32_t& port, const uint32_t& axis, const bool& inverted) : m_port(port), m_axis(axis), m_inverted(inverted) {}

float AxisJoystick::GetAmount() const
{
	return Joysticks::Get()->GetAxis(m_port, m_axis) * (m_inverted ? -1.0f : 1.0f);
}
}
