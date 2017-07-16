#pragma once

#include <stdarg.h>

#include "ibutton.h"

namespace flounder {
	/// <summary>
	/// Handles multiple buttons at once.
	/// </summary>
	class buttoncompound :
		public ibutton
	{
	private:
		int m_count;
		ibutton **m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new compound button.
		/// </summary>
		/// <param name="n_args"> The number buttons being added. </param>
		/// <param name="..."> The buttons on the being added. </param>
		buttoncompound(const int n_args, ...);

		/// <summary>
		/// Deconstructor for the compound button.
		/// </summary>
		~buttoncompound();

		bool isDown() override;

		bool wasDown() override;
	};
}
