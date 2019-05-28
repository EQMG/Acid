#pragma once

#include "Devices/Window.hpp"
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"

namespace acid
{
enum class JoystickHat :
	int32_t
{
	Centered = 0, Up = 1, Right = 2, Down = 4, Left = 8
};

ENABLE_BITMASK_OPERATORS(JoystickHat)

/**
 * @brief Module used for the creation, updating and destruction of the joysticks.
 */
class Joysticks :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Joysticks *Get() { return Engine::Get()->GetModule<Joysticks>(); }

	Joysticks();

	void Update() override;

	/**
	 * Gets if the joystick is connected.
	 * @param port The joystick to check connection with.
	 * @return If the joystick is connected.
	 */
	bool IsConnected(const uint32_t &port) const;

	/**
	 * Gets the name of the joystick.
	 * @param port The joystick to get the name of.
	 * @return The joysticks name.
	 */
	std::string GetName(const uint32_t &port) const;

	/**
	 * Gets the number of axes the joystick contains.
	 * @param port The joystick to the the axis count from.
	 * @return The number of axes the joystick contains.
	 */
	uint32_t GetAxisCount(const uint32_t &port) const;

	/**
	 * Gets the number of buttons the joystick contains.
	 * @param port The joystick to the the button count from.
	 * @return The number of buttons the joystick contains.
	 */
	uint32_t GetButtonCount(const uint32_t &port) const;

	/**
	 * Gets the number of hats the joystick contains.
	 * @param port The joystick to the the hats count from.
	 * @return The number of hats the joystick contains.
	 */
	uint32_t GetHatCount(const uint32_t &port) const;

	/**
	 * Gets the value of a joysticks axis.
	 * @param port The joystick to get the axis from.
	 * @param axis The axis id to get the value from.
	 * @return The value of the joystick's axis.
	 */
	float GetAxis(const uint32_t &port, const uint32_t &axis) const;

	/**
	 * Gets the whether a button on a joystick is pressed.
	 * @param port The joystick to get the button from.
	 * @param button The button id to get the value from.
	 * @return Whether a button on a joystick is pressed.
	 */
	InputAction GetButton(const uint32_t &port, const uint32_t &button) const;

	/**
	 * Gets the value of a joysticks hat.
	 * @param port The joystick to get the axis from.
	 * @param hat The hat id to get the value from.
	 * @return The value of the joystick's hat.
	 */
	BitMask<JoystickHat> GetHat(const uint32_t &port, const uint32_t &hat) const;

	/**
	 * Called when a joystick has been connected or disconnected.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, bool)> &OnConnect() { return m_onConnect; }

	/**
	 * Called when a joystick buttons changes state.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, uint32_t, InputAction)> &OnButton() { return m_onButton; }

	/**
	 * Called when a joystick axis moves.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, uint32_t, float)> &OnAxis() { return m_onAxis; }

	/**
	 * Called when a joystick had changes state.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, uint32_t, BitMask<JoystickHat>)> &OnHat() { return m_onHat; }

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
