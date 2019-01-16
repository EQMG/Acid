#pragma once

#include "Devices/Joysticks.hpp"
#include "IAxis.hpp"
#include "IButton.hpp"

namespace acid
{
	/// <summary>
	/// Button from a joystick.
	/// </summary>
	class ACID_EXPORT HatJoystick :
		public IAxis,
		public IButton
	{
	public:
		/// <summary>
		/// Creates a new joystick button.
		/// </summary>
		/// <param name="joystick"> The joystick port. </param>
		/// <param name="hat"> The hat that will be checked. </param>
		/// <param name="hatFlag"> If this bit is found the hat will trigger <seealso cref="#IsDown()"/>. </param>
		HatJoystick(const JoystickPort &joystick, const uint32_t &hat, const JoystickHat &hatFlag);

		float GetAmount() const override;

		bool IsDown() const override;

		bool WasDown() override;

		const JoystickPort &GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystickPort) { m_joystick = joystickPort; }

		const uint32_t &GetHat() const { return m_hat; }

		void SetHat(const uint32_t &hat) { m_hat = hat; }

		const JoystickHat &GetHatFlag() const { return m_hatFlag; }

		void SetHatFlag(const JoystickHat &hatFlag) { m_hatFlag = hatFlag; }
	private:
		JoystickPort m_joystick;
		uint32_t m_hat;
		JoystickHat m_hatFlag;
		bool m_wasDown;
	};
}
