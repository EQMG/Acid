#include "ButtonJoystick.hpp"

namespace acid
{
	ButtonJoystick::ButtonJoystick(const JoystickPort &joystick, const uint32_t &button) :
		m_joystick(joystick),
		m_button(button),
		m_wasDown(false)
	{
	}

	bool ButtonJoystick::IsDown() const
	{
		if (!Joysticks::Get()->IsConnected(m_joystick))
		{
			return false;
		}

		return Joysticks::Get()->GetButton(m_joystick, m_button);
	}

	bool ButtonJoystick::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
