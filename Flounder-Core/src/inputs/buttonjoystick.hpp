#pragma once

#include <stdarg.h>

#include "../devices/joysticks.hpp"

#include "ibutton.hpp"

namespace flounder
{
	/// <summary>
	/// Button from a joystick.
	/// </summary>
	class buttonjoystick :
		public ibutton
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
		buttonjoystick(const int &joystick, const int n_args, ...);

		/// <summary>
		/// Deconstructor for the button joystick.
		/// </summary>
		~buttonjoystick();

		bool isDown() const override;

		bool wasDown() override;
	};
}
