#pragma once

#include <vector>
#include "Joysticks.hpp"
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
		WsiJoystick m_joystick;
		std::vector<uint32_t> m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button joystick.
		/// </summary>
		/// <param name="joystick"> The joystick port. </param>
		/// <param name="buttons"> The buttons on the joystick being checked. </param>
		ButtonJoystick(const WsiJoystick &joystick, const std::vector<uint32_t> &buttons);

		/// <summary>
		/// Deconstructor for the button joystick.
		/// </summary>
		~ButtonJoystick();

		bool IsDown() const override;

		bool WasDown() override;

		WsiJoystick GetJoystick() const { return m_joystick; }

		void SetJoystick(const WsiJoystick &joystickPort) { m_joystick = joystickPort; }

		std::vector<uint32_t> GetButtons() const { return m_buttons; }

		void SetButtons(const std::vector<uint32_t> &buttons) { m_buttons = buttons; }
	};
}
