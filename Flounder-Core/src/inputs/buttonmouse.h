#pragma once

#include <stdarg.h>

#include "../devices/mouse.h"

#include "ibutton.h"

namespace flounder {
	/// <summary>
	/// Button from a mouse.
	/// </summary>
	class buttonmouse : 
		public ibutton
	{
	private:
		int m_count;
		int *m_buttons;
		bool m_wasDown;

	public:
		/// <summary>
		/// Creates a new button mouse.
		/// </summary>
		/// <param name="n_args"> The number buttons of mouse buttons being checked. </param>
		/// <param name="..."> The buttons on the mouse being checked. </param>
		buttonmouse(const int n_args, ...) :
			ibutton()
		{
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

		/// <summary>
		/// Deconstructor for the button mouse.
		/// </summary>
		~buttonmouse() 
		{
			delete m_buttons;
		}

		bool isDown() override 
		{
			if (m_count == 0)
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

		bool wasDown() override 
		{
			bool stillDown = m_wasDown && isDown();
			m_wasDown = isDown();
			return m_wasDown == !stillDown;
		}
	};
}
