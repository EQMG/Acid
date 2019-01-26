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
	public:
		/// <summary>
		/// Creates a new joystick button.
		/// </summary>
		/// <param name="port"> The joystick port. </param>
		/// <param name="button"> The button on the joystick being checked. </param>
		ButtonJoystick(const JoystickPort &port, const uint32_t &button);

		bool IsDown() const override;

		bool WasDown() override;

		const JoystickPort &GetPort() const { return m_port; }

		void SetPort(const JoystickPort &port) { m_port = port; }

		const uint32_t &GetButton() const { return m_button; }

		void SetButton(const uint32_t &button) { m_button = button; }
	private:
		JoystickPort m_port;
		uint32_t m_button;
		bool m_wasDown;
	};
}
