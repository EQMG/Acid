#include "HatJoystick.hpp"

namespace acid
{
HatJoystick::HatJoystick(const uint32_t &port, const uint32_t &hat, const BitMask<JoystickHat> &hatFlags) :
	m_port{port},
	m_hat{hat},
	m_hatFlags{hatFlags}
{
	Joysticks::Get()->OnHat().Add([this](uint32_t hat, uint32_t port, BitMask<JoystickHat> value)
	{
		if (port == m_port && hat == m_hat)
		{
			m_onAxis(GetAmount());
			auto isDown = IsDown();

			if (!m_lastDown && isDown)
			{
				m_lastDown = true;
				m_onButton(InputAction::Press, 0);
			}
			else if (m_lastDown && !isDown)
			{
				m_lastDown = false;
				m_onButton(InputAction::Release, 0);
			}
			else if (m_lastDown && isDown)
			{
				m_onButton(InputAction::Repeat, 0);
			}
		}
	}, static_cast<Button *>(this));
}

float HatJoystick::GetAmount() const
{
	auto hat = Joysticks::Get()->GetHat(m_port, m_hat);

	if (hat & JoystickHat::Up)
	{
		if (hat & JoystickHat::Right)
		{
			return 0.125f;
		}
		if (hat & JoystickHat::Left)
		{
			return 0.875f;
		}

		return 1.0f;
	}
	if (hat & JoystickHat::Down)
	{
		if (hat & JoystickHat::Right)
		{
			return 0.375f;
		}
		if (hat & JoystickHat::Left)
		{
			return 0.625f;
		}

		return 0.5f;
	}
	if (hat & JoystickHat::Right)
	{
		return 0.25f;
	}
	if (hat & JoystickHat::Left)
	{
		return 0.75f;
	}

	return 0.0f;
}

bool HatJoystick::IsDown() const
{
	return Joysticks::Get()->GetHat(m_port, m_hat) & m_hatFlags;
}
}
