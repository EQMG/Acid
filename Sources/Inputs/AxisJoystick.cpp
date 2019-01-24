#include "AxisJoystick.hpp"

#include "Maths/Maths.hpp"

namespace acid
{
	AxisJoystick::AxisJoystick(const JoystickPort &port, const uint32_t &axis, const bool &inverted) :
		m_port(port),
		m_axis(axis),
		m_inverted(inverted)
	{
	}

	float AxisJoystick::GetAmount() const
	{
		if (!Joysticks::Get()->IsConnected(m_port))
		{
			return 0.0f;
		}

		return Joysticks::Get()->GetAxis(m_port, m_axis) * (m_inverted ? -1.0f : 1.0f);
	}
}
