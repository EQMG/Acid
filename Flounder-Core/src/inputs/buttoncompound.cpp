#include "buttoncompound.h"

namespace flounder {
	buttoncompound::buttoncompound(const int n_args, ...) :
		ibutton()
	{
		m_count = n_args;
		m_buttons = new ibutton*[n_args];
		m_wasDown = false;

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_buttons[i] = va_arg(ap, ibutton*);
		}

		va_end(ap);
	}

	buttoncompound::~buttoncompound()
	{
		delete m_buttons;
	}

	bool buttoncompound::isDown()
	{
		for (int i = 0; i < m_count; i++)
		{
			if (m_buttons[i]->isDown())
			{
				return true;
			}
		}

		return false;
	}

	bool buttoncompound::wasDown()
	{
		bool stillDown = m_wasDown && isDown();
		m_wasDown = isDown();
		return m_wasDown == !stillDown;
	}
}
