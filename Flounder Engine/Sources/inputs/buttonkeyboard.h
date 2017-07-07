#pragma once

#include <stdarg.h>
#include "ibutton.h"
#include "../devices/keyboard.h"

namespace flounder {
	/// <summary>
	/// Keys from a keyboard.
	/// </summary>
	class buttonkeyboard : public ibutton
	{
	private:
		int m_count;
		int *m_keys;
		bool m_wasDown;

	public:
		/// <summary>
		/// Creates a new button keyboard.
		/// </summary>
		/// <param name="n_args"> The number keys of keyboard buttons being checked. </param>
		/// <param name="..."> The keys on the keyboard being checked. </param>
		buttonkeyboard(int n_args, ...) 
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

		/// <summary>
		/// Deconstructor for the button keyboard.
		/// </summary>
		~buttonkeyboard() 
		{
			delete m_keys;
		}

		bool isDown() override 
		{
			if (m_count == 0)
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

		bool wasDown() override 
		{
			bool stillDown = m_wasDown && isDown();
			m_wasDown = isDown();
			return m_wasDown == !stillDown;
		}
	};
}
