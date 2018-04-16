#include "ButtonJoystick.hpp"

#include "Inputs/Joysticks.hpp"

namespace Flounder
{
	ButtonJoystick::ButtonJoystick(const unsigned int &joystick, const std::vector<int> &buttons) :
		IButton(),
		m_joystick(joystick),
		m_buttons(std::vector<int>(buttons)),
		m_wasDown(false)
	{
	}

	ButtonJoystick::~ButtonJoystick()
	{
	}

	bool ButtonJoystick::IsDown() const
	{
		if (Joysticks::Get() == nullptr || !Joysticks::Get()->IsConnected(m_joystick))
		{
			return false;
		}

		for (auto button : m_buttons)
		{
			if (Joysticks::Get()->GetButton(m_joystick, button))
			{
				return true;
			}
		}

		return false;
	}

	bool ButtonJoystick::WasDown()
	{
		const bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
