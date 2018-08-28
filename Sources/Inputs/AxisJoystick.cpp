#include "AxisJoystick.hpp"

#include "Maths/Maths.hpp"

namespace acid
{
	AxisJoystick::AxisJoystick(const JoystickPort &joystick, const std::vector<uint32_t> &axes, const bool &reverse) :
		IAxis(),
		m_joystick(joystick),
		m_axes(axes),
		m_reverse(reverse)
	{
	}

	AxisJoystick::~AxisJoystick()
	{
	}

	float AxisJoystick::GetAmount() const
	{
		if (Joysticks::Get() == nullptr || !Joysticks::Get()->IsConnected(m_joystick))
		{
			return 0.0f;
		}

		float result = 0.0f;

		for (auto &axis : m_axes)
		{
			result += Joysticks::Get()->GetAxis(m_joystick, axis);
		}

		return std::clamp(result, -1.0f, 1.0f) * (m_reverse ? -1.0f : 1.0f);
	}
}
