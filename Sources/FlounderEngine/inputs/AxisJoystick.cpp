#include "AxisJoystick.hpp"

#include "../Devices/Joysticks.hpp"
#include "../Maths/Maths.hpp"

namespace Flounder
{
	AxisJoystick::AxisJoystick(const int &joystick, const std::vector<int> &axes) :
		IAxis(),
		m_joystick(joystick),
		m_axes(std::vector<int>(axes))
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

		for (auto axis : m_axes)
		{
			result += Joysticks::Get()->GetAxis(m_joystick, axis);
		}

		return Maths::Clamp(result, -1.0f, 1.0f);
	}
}
