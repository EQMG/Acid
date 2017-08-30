#include "buttonmouse.hpp"

namespace flounder
{
	buttonmouse::buttonmouse(const int n_args, ...) :
		ibutton(),
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

	buttonmouse::~buttonmouse()
	{
		delete m_buttons;
	}

	bool buttonmouse::isDown() const
	{
		if (mouse::get() == nullptr)
		{
			return false;
		}

		for (int i = 0; i < m_count; i++)
		{
			if (mouse::get()->getButton(m_buttons[i]))
			{
				return true;
			}
		}

		return false;
	}

	bool buttonmouse::wasDown()
	{
		bool stillDown = m_wasDown && isDown();
		m_wasDown = isDown();
		return m_wasDown == !stillDown;
	}
}
