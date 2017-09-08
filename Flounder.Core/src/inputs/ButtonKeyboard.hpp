#pragma once

#include <stdarg.h>

#include "../devices/Keyboard.hpp"

#include "ibutton.hpp"

namespace Flounder
{
	/// <summary>
	/// Keys from a keyboard.
	/// </summary>
	class ButtonKeyboard :
		public IButton
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
		ButtonKeyboard(const int n_args, ...);

		/// <summary>
		/// Deconstructor for the button keyboard.
		/// </summary>
		~ButtonKeyboard();

		bool IsDown() const override;

		bool WasDown() override;
	};
}
