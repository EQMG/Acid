#include "buttonmouse.hpp"

namespace Flounder
{
	ButtonMouse::ButtonMouse(const int n_args, ...) :
		IButton(),
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

	ButtonMouse::~ButtonMouse()
	{
		delete m_buttons;
	}

	bool ButtonMouse::IsDown() const
	{
		if (Mouse::Get() == nullptr)
		{
			return false;
		}

		for (int i = 0; i < m_count; i++)
		{
			if (Mouse::Get()->GetButton(m_buttons[i]))
			{
				return true;
			}
		}

		return false;
	}

	bool ButtonMouse::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
