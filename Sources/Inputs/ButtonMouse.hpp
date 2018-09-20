#pragma once

#include <vector>
#include "IButton.hpp"
#include "Mouse.hpp"

namespace acid
{
	/// <summary>
	/// Button from a mouse.
	/// </summary>
	class ACID_EXPORT ButtonMouse :
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
		explicit ButtonMouse(const std::vector<MouseButton> &buttons);

		bool IsDown() const override;

		bool WasDown() override;

		std::vector<MouseButton> GetButtons() const { return m_buttons; }

		void SetButtons(const std::vector<MouseButton> &buttons) { m_buttons = buttons; }
	};
}
