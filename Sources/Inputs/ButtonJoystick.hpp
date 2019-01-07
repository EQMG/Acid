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
		/// Creates a new joystick button.
		/// </summary>
		/// <param name="joystick"> The joystick port. </param>
		/// <param name="buttons"> The buttons on the joystick being checked. </param>
		ButtonJoystick(const JoystickPort &joystick, const std::vector<uint32_t> &buttons);

		bool IsDown() const override;

		bool WasDown() override;

		const JoystickPort &GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystickPort) { m_joystick = joystickPort; }

		const std::vector<uint32_t> &GetButtons() const { return m_buttons; }

		void SetButtons(const std::vector<uint32_t> &buttons) { m_buttons = buttons; }
	};
}
