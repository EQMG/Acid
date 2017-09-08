#include "buttonjoystick.hpp"

namespace Flounder
{
	ButtonJoystick::ButtonJoystick(const int &joystick, const int n_args, ...) :
		IButton(),
		m_joystick(joystick),
		m_count(n_args),
		m_buttons(new int[n_args]),
		m_wasDown(false)
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_buttons[i] = va_arg(ap, int);
		}

		va_end(ap);
	}

	ButtonJoystick::~ButtonJoystick()
	{
		delete m_buttons;
	}

	bool ButtonJoystick::IsDown() const
	{
		if (Joysticks::Get() == nullptr)
		{
			return false;
		}

		if (m_count == 0 || !Joysticks::Get()->IsConnected(m_joystick))
		{
			return false;
		}

		for (int i = 0; i < m_count; i++)
		{
			if (Joysticks::Get()->GetButton(m_joystick, m_buttons[i]))
			{
				return true;
			}
		}

		return false;
	}

	bool ButtonJoystick::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
