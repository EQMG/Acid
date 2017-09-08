#pragma once

#include <stdarg.h>

#include "ibutton.hpp"

namespace Flounder
{
	/// <summary>
	/// Handles multiple buttons at once.
	/// </summary>
	class ButtonCompound :
		public IButton
	{
	private:
		int m_count;
		IButton **m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new compound button.
		/// </summary>
		/// <param name="n_args"> The number buttons being added. </param>
		/// <param name="..."> The buttons on the being added. </param>
		ButtonCompound(const int n_args, ...);

		/// <summary>
		/// Deconstructor for the compound button.
		/// </summary>
		~ButtonCompound();

		bool IsDown() const override;

		bool WasDown() override;
	};
}
