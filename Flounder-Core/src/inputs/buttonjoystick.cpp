#include "buttonjoystick.h"

namespace flounder
{
	buttonjoystick::buttonjoystick(const int &joystick, const int n_args, ...) :
		ibutton()
	{
		m_joystick = joystick;
		m_count = n_args;
		m_buttons = new int[n_args];
		m_wasDown = false;

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_buttons[i] = va_arg(ap, int);
		}

		va_end(ap);
	}

	buttonjoystick::~buttonjoystick()
	{
		delete m_buttons;
	}

	bool buttonjoystick::isDown() const
	{
		if (m_count == 0 || !joysticks::get()->isConnected(m_joystick))
		{
			return false;
		}

		for (int i = 0; i < m_count; i++)
		{
			if (joysticks::get()->getButton(m_joystick, m_buttons[i]))
			{
				return true;
			}
		}

		return false;
	}

	bool buttonjoystick::wasDown()
	{
		bool stillDown = m_wasDown && isDown();
		m_wasDown = isDown();
		return m_wasDown == !stillDown;
	}
}
