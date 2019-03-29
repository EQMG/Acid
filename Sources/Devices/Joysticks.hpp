#pragma once

#include "Devices/Window.hpp"
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"

namespace acid
{
enum JoystickHat :
	int32_t
{
	Centered = 0,
	Up = 1,
	Right = 2,
	Down = 4,
	Left = 8
};

ENABLE_BITMASK_OPERATORS(JoystickHat)

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
	bool IsConnected(const uint32_t &port) const;

	/// <summary>
	/// Gets the name of the joystick.
	/// </summary>
	/// <param name="port"> The joystick to get the name of. </param>
	/// <returns> The joysticks name. </returns>
	std::string GetName(const uint32_t &port) const;

	/// <summary>
	/// Gets the number of axes the joystick offers.
	/// </summary>
	/// <param name="port"> The joystick to the the axis count from. </param>
	/// <returns> The number of axes the joystick offers. </returns>
	uint32_t GetAxisCount(const uint32_t &port) const;

	/// <summary>
	/// Gets the number of buttons the joystick offers.
	/// </summary>
	/// <param name="port"> The joystick to the the button count from. </param>
	/// <returns> The number of buttons the joystick offers. </returns>
	uint32_t GetButtonCount(const uint32_t &port) const;

	/// <summary>
	/// Gets the number of hats the joystick offers.
	/// </summary>
	/// <param name="port"> The joystick to the the hats count from. </param>
	/// <returns> The number of hats the joystick offers. </returns>
	uint32_t GetHatCount(const uint32_t &port) const;

	/// <summary>
	/// Gets the value of a joysticks axis.
	/// </summary>
	/// <param name="port"> The joystick to get the axis from. </param>
	/// <param name="axis"> The axis id to get the value from. </param>
	/// <returns> The value of the joystick's axis. </returns>
	float GetAxis(const uint32_t &port, const uint32_t &axis) const;

	/// <summary>
	/// Gets the whether a button on a joystick is pressed.
	/// </summary>
	/// <param name="port"> The joystick to get the button from. </param>
	/// <param name="button"> The button id to get the value from. </param>
	/// <returns> Whether a button on a joystick is pressed. </returns>
	InputAction GetButton(const uint32_t &port, const uint32_t &button) const;

	/// <summary>
	/// Gets the value of a joysticks axis.
	/// </summary>
	/// <param name="port"> The joystick to get the axis from. </param>
	/// <param name="hat"> The hat id to get the value from. </param>
	/// <returns> The value of the joystick's hat. </returns>
	BitMask<JoystickHat> GetHat(const uint32_t &port, const uint32_t &hat) const;

	Delegate<void(uint32_t, bool)> &GetOnConnect() { return m_onConnect; }

	Delegate<void(uint32_t, uint32_t, InputAction)> &GetOnButton() { return m_onButton; }

	Delegate<void(uint32_t, uint32_t, float)> &GetOnAxis() { return m_onAxis; }

	Delegate<void(uint32_t, uint32_t, BitMask<JoystickHat>)> &GetOnHat() { return m_onHat; }

private:
	struct JoystickImpl
	{
		std::string m_name{};
		std::vector<float> m_axes{};
		std::vector<InputAction> m_buttons{};
		std::vector<BitMask<JoystickHat>> m_hats{};
	};

	std::optional<JoystickImpl> GetJoystick(const uint32_t &port) const;

	std::map<uint32_t, JoystickImpl> m_connected;
	Delegate<void(uint32_t, bool)> m_onConnect;
	Delegate<void(uint32_t, uint32_t, InputAction)> m_onButton;
	Delegate<void(uint32_t, uint32_t, float)> m_onAxis;
	Delegate<void(uint32_t, uint32_t, BitMask<JoystickHat>)> m_onHat;

	friend void CallbackJoystick(int32_t id, int32_t event);
};
}
