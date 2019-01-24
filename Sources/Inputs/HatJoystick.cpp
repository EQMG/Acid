#include "HatJoystick.hpp"

namespace acid
{
	HatJoystick::HatJoystick(const JoystickPort &port, const uint32_t &hat, const JoystickHat &hatFlag) :
		m_port(port),
		m_hat(hat),
		m_hatFlag(hatFlag),
		m_wasDown(false)
	{
	}

	float HatJoystick::GetAmount() const
	{
		if (!Joysticks::Get()->IsConnected(m_port))
		{
			return 0.0f;
		}

		switch (Joysticks::Get()->GetHat(m_port, m_hat))
		{
			case JOYSTICK_HAT_CENTERED:
				return 0.0f;
			case JOYSTICK_HAT_UP:
				return 0.0f;
			case JOYSTICK_HAT_RIGHT_UP:
				return 0.125f;
			case JOYSTICK_HAT_RIGHT:
				return 0.25f;
			case JOYSTICK_HAT_RIGHT_DOWN:
				return 0.375f;
			case JOYSTICK_HAT_DOWN:
				return 0.5f;
			case JOYSTICK_HAT_LEFT_DOWN:
				return 0.625f;
			case JOYSTICK_HAT_LEFT:
				return 0.75f;
			case JOYSTICK_HAT_LEFT_UP:
				return 1.0f;
			default:
				return 0.0f;
		}
	}

	bool HatJoystick::IsDown() const
	{
		if (!Joysticks::Get()->IsConnected(m_port))
		{
			return false;
		}

		return (Joysticks::Get()->GetHat(m_port, m_hat) & m_hatFlag) == 1;
	}

	bool HatJoystick::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
