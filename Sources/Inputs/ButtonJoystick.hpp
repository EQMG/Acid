#pragma once

#include "Devices/Joysticks.hpp"
#include "IButton.hpp"

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
		uint32_t m_button;
		bool m_wasDown;
	public:
		/// <summary>
		/// Creates a new joystick button.
		/// </summary>
		/// <param name="joystick"> The joystick port. </param>
		/// <param name="button"> The button on the joystick being checked. </param>
		ButtonJoystick(const JoystickPort &joystick, const uint32_t &button);

		bool IsDown() const override;

		bool WasDown() override;

		const JoystickPort &GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystickPort) { m_joystick = joystickPort; }

		const uint32_t &GetButton() const { return m_button; }

		void SetButton(const uint32_t &button) { m_button = button; }
	};
}
