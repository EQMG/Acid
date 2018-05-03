#pragma once

#include <vector>
#include "IButton.hpp"

namespace fl
{
	/// <summary>
	/// Button from a mouse.
	/// </summary>
	class FL_EXPORT ButtonMouse :
		public IButton
	{
	private:
		std::vector<int> m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button mouse.
		/// </summary>
		/// <param name="buttons"> The buttons on the mouse being checked. </param>
		ButtonMouse(const std::vector<int> &buttons);

		/// <summary>
		/// Deconstructor for the button mouse.
		/// </summary>
		~ButtonMouse();

		bool IsDown() const override;

		bool WasDown() override;
	};
}
