#include "buttonkeyboard.hpp"

namespace Flounder
{
	ButtonKeyboard::ButtonKeyboard(const int n_args, ...) :
		IButton(),
		m_count(n_args),
		m_keys(new int[n_args]),
		m_wasDown(false)
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_keys[i] = va_arg(ap, int);
		}

		va_end(ap);
	}

	ButtonKeyboard::~ButtonKeyboard()
	{
		delete m_keys;
	}

	bool ButtonKeyboard::IsDown() const
	{
		if (Keyboard::Get() == nullptr)
		{
			return false;
		}

		for (int i = 0; i < m_count; i++)
		{
			if (Keyboard::Get()->GetKey(m_keys[i]))
			{
				return true;
			}
		}

		return false;
	}

	bool ButtonKeyboard::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
