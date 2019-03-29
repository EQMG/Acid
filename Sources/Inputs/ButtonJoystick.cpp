#include "ButtonJoystick.hpp"

#include "Devices/Joysticks.hpp"

namespace acid
{
	ButtonJoystick::ButtonJoystick(const uint32_t &port, const uint32_t &button) :
		m_port(port),
		m_button(button),
		m_wasDown(false)
	{
		Joysticks::Get()->GetOnButton() += [this](uint32_t button, uint32_t port, InputAction action)
		{
			if (port == m_port && button == m_button)
			{
				m_onButton(action, 0);
			}
		};
	}

	bool ButtonJoystick::IsDown() const
	{
		return Joysticks::Get()->GetButton(m_port, m_button) != InputAction::Release;
	}

	bool ButtonJoystick::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
