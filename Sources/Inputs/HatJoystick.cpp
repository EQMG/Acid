#include "HatJoystick.hpp"

namespace acid
{
HatJoystick::HatJoystick(const uint32_t& port, const uint32_t& hat, const JoystickHat& hatFlag) : m_port(port), m_hat(hat), m_hatFlag(hatFlag), m_wasDown(false) {}

float HatJoystick::GetAmount() const
{
	auto hat = Joysticks::Get()->GetHat(m_port, m_hat);

	if(hat & Up)
		{
			if(hat & Right)
				{
					return 0.125f;
				}
			if(hat & Left)
				{
					return 0.875f;
				}

			return 1.0f;
		}
	if(hat & Down)
		{
			if(hat & Right)
				{
					return 0.375f;
				}
			if(hat & Left)
				{
					return 0.625f;
				}

			return 0.5f;
		}
	if(hat & Right)
		{
			return 0.25f;
		}
	if(hat & Left)
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
