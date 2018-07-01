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
		std::vector<WsiMouseButton> m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button mouse.
		/// </summary>
		/// <param name="buttons"> The buttons on the mouse being checked. </param>
		ButtonMouse(const std::vector<WsiMouseButton> &buttons);

		/// <summary>
		/// Deconstructor for the button mouse.
		/// </summary>
		~ButtonMouse();

		bool IsDown() const override;

		bool WasDown() override;

		std::vector<WsiMouseButton> GetButtons() const { return m_buttons; }

		void SetButtons(const std::vector<WsiMouseButton> &buttons) { m_buttons = buttons; }
	};
}
