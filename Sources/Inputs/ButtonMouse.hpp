#pragma once

#include <vector>
#include "Mouse.hpp"
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
		std::vector<MouseButton> m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button mouse.
		/// </summary>
		/// <param name="buttons"> The buttons on the mouse being checked. </param>
		ButtonMouse(const std::vector<MouseButton> &buttons);

		/// <summary>
		/// Deconstructor for the button mouse.
		/// </summary>
		~ButtonMouse();

		bool IsDown() const override;

		bool WasDown() override;

		std::vector<MouseButton> GetButtons() const { return m_buttons; }

		void SetButtons(const std::vector<MouseButton> &buttons) { m_buttons = buttons; }
	};
}
