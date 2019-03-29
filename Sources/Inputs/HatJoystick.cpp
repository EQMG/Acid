#include "HatJoystick.hpp"

namespace acid
{
HatJoystick::HatJoystick(const uint32_t &port, const uint32_t &hat, const JoystickHat &hatFlag) :
	m_port(port),
	m_hat(hat),
	m_hatFlag(hatFlag),
	m_wasDown(false)
{
	Joysticks::Get()->GetOnHat() += [this](uint32_t hat, uint32_t port, BitMask<JoystickHat> value)
	{
		if (port == m_port && hat == m_hat)
		{
			m_onAxis(GetAmount());

			if (!m_wasDown && value & m_hatFlag)
			{
				m_onButton(InputAction::Press, 0);
			}
			else if (m_wasDown && !(value & m_hatFlag))
			{
				m_onButton(InputAction::Release, 0);
			}
			else
			{
				m_onButton(InputAction::Repeat, 0);
			}
		}
	};
}

float HatJoystick::GetAmount() const
{
	auto hat = Joysticks::Get()->GetHat(m_port, m_hat);

	if (hat & Up)
	{
		if (hat & Right)
		{
			return 0.125f;
		}
		if (hat & Left)
		{
			return 0.875f;
		}

		return 1.0f;
	}
	if (hat & Down)
	{
		if (hat & Right)
		{
			return 0.375f;
		}
		if (hat & Left)
		{
			return 0.625f;
		}

		return 0.5f;
	}
	if (hat & Right)
	{
		return 0.25f;
	}
	if (hat & Left)
	{
		return 0.75f;
	}

	return 0.0f;
}

bool HatJoystick::IsDown() const
{
	return Joysticks::Get()->GetHat(m_port, m_hat) & m_hatFlag;
}

bool HatJoystick::WasDown()
{
	bool stillDown = m_wasDown && IsDown();
	m_wasDown = IsDown();
	return m_wasDown == !stillDown;
}
}
