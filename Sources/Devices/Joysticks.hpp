#pragma once

#include "Devices/Window.hpp"
#include "Engine/Engine.hpp"

namespace acid {
enum class JoystickHatValue : uint8_t {
	Centered = 0,
	Up = 1,
	Right = 2,
	Down = 4,
	Left = 8
};

ENABLE_BITMASK_OPERATORS(JoystickHatValue);

using JoystickPort = uint8_t;
using JoystickAxis = uint8_t;
using JoystickButton = uint8_t;
using JoystickHat = uint8_t;

/**
 * @brief Module used for the creation, updating and destruction of the joysticks.
 */
class ACID_EXPORT Joysticks : public Module::Registrar<Joysticks> {
	inline static const bool Registered = Register(Stage::Pre, Requires<Window>());
public:
	Joysticks();

	void Update() override;

	/**
	 * Gets if the joystick is connected.
	 * @param port The joystick to check connection with.
	 * @return If the joystick is connected.
	 */
	bool IsConnected(JoystickPort port) const;

	/**
	 * Gets the name of the joystick.
	 * @param port The joystick to get the name of.
	 * @return The joysticks name.
	 */
	std::string GetName(JoystickPort port) const;

	/**
	 * Gets the number of axes the joystick contains.
	 * @param port The joystick to the the axis count from.
	 * @return The number of axes the joystick contains.
	 */
	std::size_t GetAxisCount(JoystickPort port) const;

	/**
	 * Gets the number of buttons the joystick contains.
	 * @param port The joystick to the the button count from.
	 * @return The number of buttons the joystick contains.
	 */
	std::size_t GetButtonCount(JoystickPort port) const;

	/**
	 * Gets the number of hats the joystick contains.
	 * @param port The joystick to the the hats count from.
	 * @return The number of hats the joystick contains.
	 */
	std::size_t GetHatCount(JoystickPort port) const;

	/**
	 * Gets the value of a joysticks axis.
	 * @param port The joystick to get the axis from.
	 * @param axis The axis id to get the value from.
	 * @return The value of the joystick's axis.
	 */
	float GetAxis(JoystickPort port, uint8_t axis) const;

	/**
	 * Gets the whether a button on a joystick is pressed.
	 * @param port The joystick to get the button from.
	 * @param button The button id to get the value from.
	 * @return Whether a button on a joystick is pressed.
	 */
	InputAction GetButton(JoystickPort port, uint8_t button) const;

	/**
	 * Gets the value of a joysticks hat.
	 * @param port The joystick to get the axis from.
	 * @param hat The hat id to get the value from.
	 * @return The value of the joystick's hat.
	 */
	BitMask<JoystickHatValue> GetHat(JoystickPort port, uint8_t hat) const;

	/**
	 * Called when a joystick has been connected or disconnected.
	 * @return The delegate.
	 */
	rocket::signal<void(JoystickPort, bool)> &OnConnect() { return onConnect; }

	/**
	 * Called when a joystick buttons changes state.
	 * @return The delegate.
	 */
	rocket::signal<void(JoystickPort, uint8_t, InputAction)> &OnButton() { return onButton; }

	/**
	 * Called when a joystick axis moves.
	 * @return The delegate.
	 */
	rocket::signal<void(JoystickPort, uint8_t, float)> &OnAxis() { return onAxis; }

	/**
	 * Called when a joystick had changes state.
	 * @return The delegate.
	 */
	rocket::signal<void(JoystickPort, uint8_t, BitMask<JoystickHatValue>)> &OnHat() { return onHat; }

private:
	class JoystickImpl {
	public:
		std::string name;
		std::vector<float> axes;
		std::vector<InputAction> buttons;
		std::vector<BitMask<JoystickHatValue>> hats;
	};

	std::optional<JoystickImpl> GetJoystick(JoystickPort port) const;

	friend void CallbackJoystick(int32_t id, int32_t event);

	std::map<JoystickPort, JoystickImpl> connected;
	rocket::signal<void(JoystickPort, bool)> onConnect;
	rocket::signal<void(JoystickPort, uint8_t, InputAction)> onButton;
	rocket::signal<void(JoystickPort, uint8_t, float)> onAxis;
	rocket::signal<void(JoystickPort, uint8_t, BitMask<JoystickHatValue>)> onHat;
};
}
