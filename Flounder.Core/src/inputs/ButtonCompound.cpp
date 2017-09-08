#include "buttoncompound.hpp"

namespace Flounder
{
	ButtonCompound::ButtonCompound(const int n_args, ...) :
		IButton(),
		m_count(n_args),
		m_buttons(new IButton*[n_args]),
		m_wasDown(false)
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_buttons[i] = va_arg(ap, IButton*);
		}

		va_end(ap);
	}

	ButtonCompound::~ButtonCompound()
	{
		delete m_buttons;
	}

	bool ButtonCompound::IsDown() const
	{
		for (int i = 0; i < m_count; i++)
		{
			if (m_buttons[i]->IsDown())
			{
				return true;
			}
		}

		return false;
	}

	bool ButtonCompound::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
