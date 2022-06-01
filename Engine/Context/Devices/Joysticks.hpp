#pragma once

#include "Engine/Engine.hpp"
#include "Windows.hpp"

namespace acid {
enum class JoystickHatValue : uint8_t {
	Centered = 0,
	Up = 1,
	Right = 2,
	Down = 4,
	Left = 8
};
ENABLE_BITMASK_OPERATORS(JoystickHatValue)

enum class JoystickPort : uint8_t {
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
	_16 = 15
};

using JoystickAxis = uint8_t;
using JoystickButton = uint8_t;
using JoystickHat = uint8_t;

class ACID_CONTEXT_EXPORT Joystick {
	friend class Joysticks;
public:
	void Update();

	JoystickPort GetPort() const { return port; }

	/**
	 * Gets if the joystick is connected.
	 * @param port The joystick to check connection with.
	 * @return If the joystick is connected.
	 */
	bool IsConnected() const { return connected; }

	/**
	 * Gets the name of the joystick.
	 * @param port The joystick to get the name of.
	 * @return The joysticks name.
	 */
	std::string GetName() const { return name; }

	/**
	 * Gets the number of axes the joystick contains.
	 * @param port The joystick to the the axis count from.
	 * @return The number of axes the joystick contains.
	 */
	std::size_t GetAxisCount() const { return axes.size(); }

	/**
	 * Gets the number of buttons the joystick contains.
	 * @param port The joystick to the the button count from.
	 * @return The number of buttons the joystick contains.
	 */
	std::size_t GetButtonCount() const { return buttons.size(); }

	/**
	 * Gets the number of hats the joystick contains.
	 * @param port The joystick to the the hats count from.
	 * @return The number of hats the joystick contains.
	 */
	std::size_t GetHatCount() const { return hats.size(); }

	/**
	 * Gets the value of a joysticks axis.
	 * @param port The joystick to get the axis from.
	 * @param axis The axis id to get the value from.
	 * @return The value of the joystick's axis.
	 */
	float GetAxis(uint8_t axis) const;

	/**
	 * Gets the whether a button on a joystick is pressed.
	 * @param port The joystick to get the button from.
	 * @param button The button id to get the value from.
	 * @return Whether a button on a joystick is pressed.
	 */
	InputAction GetButton(uint8_t button) const;

	/**
	 * Gets the value of a joysticks hat.
	 * @param port The joystick to get the axis from.
	 * @param hat The hat id to get the value from.
	 * @return The value of the joystick's hat.
	 */
	bitmask::bitmask<JoystickHatValue> GetHat(uint8_t hat) const;

	/**
	 * Called when a joystick has been connected or disconnected.
	 * @return The delegate.
	 */
	rocket::signal<void(bool)> &OnConnect() { return onConnect; }

	/**
	 * Called when a joystick buttons changes state.
	 * @return The delegate.
	 */
	rocket::signal<void(uint8_t, InputAction)> &OnButton() { return onButton; }

	/**
	 * Called when a joystick axis moves.
	 * @return The delegate.
	 */
	rocket::signal<void(uint8_t, float)> &OnAxis() { return onAxis; }

	/**
	 * Called when a joystick had changes state.
	 * @return The delegate.
	 */
	rocket::signal<void(uint8_t, bitmask::bitmask<JoystickHatValue>)> &OnHat() { return onHat; }

private:
	friend void CallbackJoystick(int32_t id, int32_t event);
	
	JoystickPort port;
	bool connected = false;
	std::string name;
	std::vector<float> axes;
	std::vector<InputAction> buttons;
	std::vector<bitmask::bitmask<JoystickHatValue>> hats;

	rocket::signal<void(bool)> onConnect;
	rocket::signal<void(uint8_t, InputAction)> onButton;
	rocket::signal<void(uint8_t, float)> onAxis;
	rocket::signal<void(uint8_t, bitmask::bitmask<JoystickHatValue>)> onHat;
};

/**
 * @brief Module used for the creation, updating and destruction of the joysticks.
 */
class ACID_CONTEXT_EXPORT Joysticks : public Module::Registrar<Joysticks> {
	inline static const bool Registered = Register(Stage::Pre, Requires<Windows>());
public:
	Joysticks();

	void Update() override;

	const Joystick *GetJoystick(JoystickPort port) const;
	Joystick *GetJoystick(JoystickPort port);

private:
	std::array<Joystick, magic_enum::enum_count<JoystickPort>()> joysticks;
};
}
