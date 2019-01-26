#include "HatJoystick.hpp"

namespace acid
{
	HatJoystick::HatJoystick(const uint32_t &port, const uint32_t &hat, const JoystickHat &hatFlag) :
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

		auto hat = Joysticks::Get()->GetHat(m_port, m_hat);

		if (hat & JoystickHat::Up)
		{
			if (hat & JoystickHat::Right)
			{
				return 0.125f;
			}
			else if (hat & JoystickHat::Left)
			{
				return 0.875f;
			}

			return 1.0f;
		}
		else if (hat & JoystickHat::Down)
		{
			if (hat & JoystickHat::Right)
			{
				return 0.375f;
			}
			else if (hat & JoystickHat::Left)
			{
				return 0.625f;
			}

			return 0.5f;
		}
		else if (hat & JoystickHat::Right)
		{
			return 0.25f;
		}
		else if (hat & JoystickHat::Left)
		{
			return 0.75f;
		}

		return 0.0f;
	}

	bool HatJoystick::IsDown() const
	{
		if (!Joysticks::Get()->IsConnected(m_port))
		{
			return false;
		}

		return Joysticks::Get()->GetHat(m_port, m_hat) & m_hatFlag;
	}

	bool HatJoystick::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
