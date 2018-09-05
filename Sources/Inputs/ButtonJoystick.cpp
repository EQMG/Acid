#include "ButtonJoystick.hpp"

namespace acid
{
	ButtonJoystick::ButtonJoystick(const JoystickPort &joystick, const std::vector<uint32_t> &buttons) :
		m_joystick(joystick),
		m_buttons(buttons),
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

		for (auto &button : m_buttons)
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
