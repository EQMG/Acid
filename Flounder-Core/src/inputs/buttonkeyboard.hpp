#pragma once

#include <stdarg.h>

#include "../devices/Keyboard.hpp"

#include "ibutton.hpp"

namespace Flounder
{
	/// <summary>
	/// Keys from a keyboard.
	/// </summary>
	class buttonkeyboard :
		public ibutton
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
		buttonkeyboard(const int n_args, ...);

		/// <summary>
		/// Deconstructor for the button keyboard.
		/// </summary>
		~buttonkeyboard();

		bool isDown() const override;

		bool wasDown() override;
	};
}
