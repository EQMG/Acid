#pragma once

#include <stdarg.h>

#include "../devices/joysticks.h"

#include "ibutton.h"

namespace flounder {
	/// <summary>
	/// Button from a joystick.
	/// </summary>
	class buttonjoystick : public ibutton
	{
	private:
		int m_joystick;
		int m_count;
		int *m_buttons;
		bool m_wasDown;

	public:
		/// <summary>
		/// Creates a new button joystick.
		/// </summary>
		/// <param name="joystick"> The joystick. Should be one of the GLFW.JOYSTICK values. </param>
		/// <param name="n_args"> The number buttons of joystick buttons being checked. </param>
		/// <param name="..."> The buttons on the joystick being checked. </param>
		buttonjoystick(const int joystick, const int n_args, ...)
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

		/// <summary>
		/// Deconstructor for the button joystick.
		/// </summary>
		~buttonjoystick() 
		{
			delete m_buttons;
		}

		bool isDown() override {
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

		bool wasDown() override 
		{
			bool stillDown = m_wasDown && isDown();
			m_wasDown = isDown();
			return m_wasDown == !stillDown;
		}
	};
}
