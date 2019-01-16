#pragma once

#include <array>
#include "Devices/Window.hpp"
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"

namespace acid
{
	enum JoystickPort
	{
		JOYSTICK_1 = 0,
		JOYSTICK_2 = 1,
		JOYSTICK_3 = 2,
		JOYSTICK_4 = 3,
		JOYSTICK_5 = 4,
		JOYSTICK_6 = 5,
		JOYSTICK_7 = 6,
		JOYSTICK_8 = 7,
		JOYSTICK_9 = 8,
		JOYSTICK_10 = 9,
		JOYSTICK_11 = 10,
		JOYSTICK_12 = 11,
		JOYSTICK_13 = 12,
		JOYSTICK_14 = 13,
		JOYSTICK_15 = 14,
		JOYSTICK_16 = 15,
		JOYSTICK_END_RANGE = JOYSTICK_16
	};

	enum JoystickHat
	{
		JOYSTICK_HAT_CENTERED = 0,
		JOYSTICK_HAT_UP = 1,
		JOYSTICK_HAT_RIGHT = 2,
		JOYSTICK_HAT_DOWN = 4,
		JOYSTICK_HAT_LEFT = 8,
		JOYSTICK_HAT_RIGHT_UP = JOYSTICK_HAT_RIGHT | JOYSTICK_HAT_UP,
		JOYSTICK_HAT_RIGHT_DOWN = JOYSTICK_HAT_RIGHT | JOYSTICK_HAT_DOWN,
		JOYSTICK_HAT_LEFT_UP = JOYSTICK_HAT_LEFT  | JOYSTICK_HAT_UP,
		JOYSTICK_HAT_LEFT_DOWN = JOYSTICK_HAT_LEFT  | JOYSTICK_HAT_DOWN
	};
	typedef uint32_t JoystickHatFlags;

	/// <summary>
	/// A module used for the creation, updating and destruction of the joysticks.
	/// </summary>
	class ACID_EXPORT Joysticks :
		public Module
	{
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Joysticks *Get() { return Engine::Get()->GetModuleManager().Get<Joysticks>(); }

		Joysticks();

		void Update() override;

		/// <summary>
		/// Determines if the joystick is connected
		/// </summary>
		/// <param name="port"> The joystick to check connection with. </param>
		/// <returns> If the joystick is connected. </returns>
		const bool &IsConnected(const JoystickPort &port) const { return m_connected.at(port).m_connected; }

		/// <summary>
		/// Gets the name of the joystick.
		/// </summary>
		/// <param name="port"> The joystick to get the name of. </param>
		/// <returns> The joysticks name. </returns>
		const std::string &GetName(const JoystickPort &port) const { return m_connected.at(port).m_name; }

		/// <summary>
		/// Gets the value of a joysticks axis.
		/// </summary>
		/// <param name="port"> The joystick to get the axis from. </param>
		/// <param name="axis"> The axis id to get the value from. </param>
		/// <returns> The value of the joystick's axis. </returns>
		float GetAxis(const JoystickPort &port, const uint32_t &axis) const;

		/// <summary>
		/// Gets the whether a button on a joystick is pressed.
		/// </summary>
		/// <param name="port"> The joystick to get the button from. </param>
		/// <param name="button"> The button id to get the value from. </param>
		/// <returns> Whether a button on a joystick is pressed. </returns>
		InputAction GetButton(const JoystickPort &port, const uint32_t &button) const;

		/// <summary>
		/// Gets the value of a joysticks axis.
		/// </summary>
		/// <param name="port"> The joystick to get the axis from. </param>
		/// <param name="hat"> The hat id to get the value from. </param>
		/// <returns> The value of the joystick's hat. </returns>
		JoystickHatFlags GetHat(const JoystickPort &port, const uint32_t &hat) const;

		/// <summary>
		/// Gets the number of axes the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the axis count from. </param>
		/// <returns> The number of axes the joystick offers. </returns>
		const uint32_t &GetAxisCount(const JoystickPort &port) const { return m_connected.at(port).m_axeCount; }

		/// <summary>
		/// Gets the number of buttons the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the button count from. </param>
		/// <returns> The number of buttons the joystick offers. </returns>
		const uint32_t &GetButtonCount(const JoystickPort &port) const { return m_connected.at(port).m_buttonCount; }

		/// <summary>
		/// Gets the number of hats the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the hats count from. </param>
		/// <returns> The number of hats the joystick offers. </returns>
		const uint32_t &GetHatCount(const JoystickPort &port) const { return m_connected.at(port).m_hatCount; }

		Delegate<void(JoystickPort, bool)> &OnConnect() { return m_onConnect; }
	private:
		struct JoystickImpl
		{
			JoystickPort m_port;
			bool m_connected;
			bool m_gamepad;
			std::string m_name;

			const float *m_axes;
			uint32_t m_axeCount;
			const uint8_t *m_buttons;
			uint32_t m_buttonCount;
			const uint8_t *m_hats;
			uint32_t m_hatCount;
		};

		std::array<JoystickImpl, JOYSTICK_END_RANGE> m_connected;
		Delegate<void(JoystickPort, bool)> m_onConnect;

		friend void CallbackJoystick(int32_t id, int32_t event);
	};
}
