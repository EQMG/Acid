#pragma once

#include <vector>
#include "IButton.hpp"
#include "Joysticks.hpp"

namespace acid
{
	/// <summary>
	/// Button from a joystick.
	/// </summary>
	class ACID_EXPORT ButtonJoystick :
		public IButton
	{
	private:
		JoystickPort m_joystick;
		std::vector<uint32_t> m_buttons;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new button joystick.
		/// </summary>
		/// <param name="joystick"> The joystick port. </param>
		/// <param name="buttons"> The buttons on the joystick being checked. </param>
		ButtonJoystick(const JoystickPort &joystick, const std::vector<uint32_t> &buttons);

		/// <summary>
		/// Deconstructor for the button joystick.
		/// </summary>
		~ButtonJoystick();

		bool IsDown() const override;

		bool WasDown() override;

		JoystickPort GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystickPort) { m_joystick = joystickPort; }

		std::vector<uint32_t> GetButtons() const { return m_buttons; }

		void SetButtons(const std::vector<uint32_t> &buttons) { m_buttons = buttons; }
	};
}
