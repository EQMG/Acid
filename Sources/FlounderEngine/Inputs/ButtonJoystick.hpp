#pragma once

#include <vector>
#include "IButton.hpp"

namespace fl
{
	/// <summary>
	/// Button from a joystick.
	/// </summary>
	class FL_EXPORT ButtonJoystick :
		public IButton
	{
	private:
		unsigned int m_joystick;
		std::vector<int> m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button joystick.
		/// </summary>
		/// <param name="joystick"> The joystick. Should be one of the GLFW.JOYSTICK values. </param>
		/// <param name="buttons"> The buttons on the joystick being checked. </param>
		ButtonJoystick(const unsigned int &joystick, const std::vector<int> &buttons);

		/// <summary>
		/// Deconstructor for the button joystick.
		/// </summary>
		~ButtonJoystick();

		bool IsDown() const override;

		bool WasDown() override;
	};
}
