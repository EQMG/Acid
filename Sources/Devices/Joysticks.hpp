#pragma once

#include <array>
#include "Devices/Window.hpp"
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"

namespace acid
{
	enum class JoystickPort
	{
		_1 = 0,
		_2 = 1,
		_3 = 2,
		_4 = 3,
		_5 = 4,
		_6 = 5,
		_7 = 6,
		_8 = 7,
		_9 = 8,
		_10 = 9,
		_11 = 10,
		_12 = 11,
		_13 = 12,
		_14 = 13,
		_15 = 14,
		_16 = 15,
		First = _1,
		Last = _16
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
		const bool &IsConnected(const JoystickPort &port) const { return m_connected.at(static_cast<uint32_t>(port)).m_connected; }

		/// <summary>
		/// Gets the name of the joystick.
		/// </summary>
		/// <param name="port"> The joystick to get the name of. </param>
		/// <returns> The joysticks name. </returns>
		const std::string &GetName(const JoystickPort &port) const { return m_connected.at(static_cast<uint32_t>(port)).m_name; }

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
		JoystickHat GetHat(const JoystickPort &port, const uint32_t &hat) const;

		/// <summary>
		/// Gets the number of axes the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the axis count from. </param>
		/// <returns> The number of axes the joystick offers. </returns>
		uint32_t GetAxisCount(const JoystickPort &port) const { return m_connected.at(static_cast<uint32_t>(port)).m_axes.size(); }

		/// <summary>
		/// Gets the number of buttons the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the button count from. </param>
		/// <returns> The number of buttons the joystick offers. </returns>
		uint32_t GetButtonCount(const JoystickPort &port) const { return m_connected.at(static_cast<uint32_t>(port)).m_buttons.size(); }

		/// <summary>
		/// Gets the number of hats the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the hats count from. </param>
		/// <returns> The number of hats the joystick offers. </returns>
		uint32_t GetHatCount(const JoystickPort &port) const { return m_connected.at(static_cast<uint32_t>(port)).m_hats.size(); }

		Delegate<void(JoystickPort, bool)> &GetOnConnect() { return m_onConnect; }

		Delegate<void(JoystickPort, uint32_t, InputAction)> &GetOnButton() { return m_onButton; }

		Delegate<void(JoystickPort, uint32_t, float)> &GetOnAxis() { return m_onAxis; }

		Delegate<void(JoystickPort, uint32_t, JoystickHat)> &GetOnHat() { return m_onHat; }
	private:
		struct JoystickImpl
		{
			JoystickPort m_port;
			bool m_connected;
			std::string m_name;

			std::vector<float> m_axes;
			std::vector<InputAction> m_buttons;
			std::vector<JoystickHat> m_hats;
		};

		std::array<JoystickImpl, 16> m_connected;
		Delegate<void(JoystickPort, bool)> m_onConnect;
		Delegate<void(JoystickPort, uint32_t, InputAction)> m_onButton;
		Delegate<void(JoystickPort, uint32_t, float)> m_onAxis;
		Delegate<void(JoystickPort, uint32_t, JoystickHat)> m_onHat;

		friend void CallbackJoystick(int32_t id, int32_t event);
	};
}
