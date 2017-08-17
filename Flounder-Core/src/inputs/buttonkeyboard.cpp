#include "buttonkeyboard.h"

namespace flounder
{
	buttonkeyboard::buttonkeyboard(const int n_args, ...) :
		ibutton()
	{
		m_count = n_args;
		m_keys = new int[n_args];
		m_wasDown = false;

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_keys[i] = va_arg(ap, int);
		}

		va_end(ap);
	}

	buttonkeyboard::~buttonkeyboard()
	{
		delete m_keys;
	}

	bool buttonkeyboard::isDown() const
	{
		if (keyboard::get() == nullptr)
		{
			return false;
		}

		for (int i = 0; i < m_count; i++)
		{
			if (keyboard::get()->getKey(m_keys[i]))
			{
				return true;
			}
		}

		return false;
	}

	bool buttonkeyboard::wasDown()
	{
		bool stillDown = m_wasDown && isDown();
		m_wasDown = isDown();
		return m_wasDown == !stillDown;
	}
}
