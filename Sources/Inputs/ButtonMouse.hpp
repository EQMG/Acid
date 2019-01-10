#pragma once

#include "Devices/Mouse.hpp"
#include "IButton.hpp"

namespace acid
{
	/// <summary>
	/// Button from a mouse.
	/// </summary>
	class ACID_EXPORT ButtonMouse :
		public IButton
	{
	private:
		MouseButton m_button;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button mouse.
		/// </summary>
		/// <param name="buttons"> The button on the mouse being checked. </param>
		explicit ButtonMouse(const MouseButton &button);

		bool IsDown() const override;

		bool WasDown() override;

		const MouseButton &GetButton() const { return m_button; }

		void SetButton(const MouseButton &button) { m_button = button; }
	};
}
