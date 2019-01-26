#include "ButtonJoystick.hpp"

namespace acid
{
	ButtonJoystick::ButtonJoystick(const JoystickPort &port, const uint32_t &button) :
		m_port(port),
		m_button(button),
		m_wasDown(false)
	{
	}

	bool ButtonJoystick::IsDown() const
	{
		if (!Joysticks::Get()->IsConnected(m_port))
		{
			return false;
		}

		return Joysticks::Get()->GetButton(m_port, m_button) != InputAction::Release;
	}

	bool ButtonJoystick::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
