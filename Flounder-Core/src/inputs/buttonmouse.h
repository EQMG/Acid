#pragma once

#include <stdarg.h>

#include "../devices/mouse.h"

#include "ibutton.h"

namespace flounder 
{
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
		buttonmouse(const int n_args, ...);

		/// <summary>
		/// Deconstructor for the button mouse.
		/// </summary>
		~buttonmouse();

		bool isDown() const override;

		bool wasDown();
	};
}
