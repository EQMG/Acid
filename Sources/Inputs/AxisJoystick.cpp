#include "AxisJoystick.hpp"

#include "Maths/Maths.hpp"

namespace acid
{
	AxisJoystick::AxisJoystick(const JoystickPort &joystick, const uint32_t &axis, const bool &inverted) :
		m_joystick(joystick),
		m_axis(axis),
		m_inverted(inverted)
	{
	}

	float AxisJoystick::GetAmount() const
	{
		if (!Joysticks::Get()->IsConnected(m_joystick))
		{
			return 0.0f;
		}

		return Joysticks::Get()->GetAxis(m_joystick, m_axis) * (m_inverted ? -1.0f : 1.0f);
	}
}
