#include "Joysticks.hpp"

#include <GLFW/glfw3.h>

namespace acid {
static_assert(GLFW_JOYSTICK_LAST == static_cast<int16_t>(JoystickPort::Last), "GLFW joystick port count does not match our joystick port enum count.");

const bool Joysticks::Registered = Register(Stage::Pre, Requires<Window>());

void CallbackJoystick(int32_t id, int32_t event) {
	if (event == GLFW_CONNECTED) {
		Log::Out("Joystick connected: '", glfwGetJoystickName(id), "' to ", id, '\n');
		Joysticks::Get()->joysticks[id] = {true, glfwGetJoystickName(id)};
		Joysticks::Get()->onConnect((JoystickPort)id, true);
	} else if (event == GLFW_DISCONNECTED) {
		Log::Out("Joystick disconnected from ", id, '\n');
		Joysticks::Get()->joysticks[id] = {false};
		Joysticks::Get()->onConnect((JoystickPort)id, false);
	}
}

Joysticks::Joysticks() {
	glfwSetJoystickCallback(CallbackJoystick);

	for (auto &&[port, joystick] : Enumerate(joysticks)) {
		if (glfwJoystickPresent(port)) {
			Joystick joystick = {};
			joystick.name = glfwGetJoystickName(port);
			joysticks[port] = joystick;
			onConnect((JoystickPort)port, true);
		}
	}
}

void Joysticks::Update() {
	for (auto &&[port, joystick] : Enumerate(joysticks)) {
		if (!joystick.connected) continue;
		
		int32_t axeCount = 0;
		auto axes = glfwGetJoystickAxes(port, &axeCount);
		joystick.axes.resize(static_cast<std::size_t>(axeCount));

		for (uint32_t i = 0; i < static_cast<uint32_t>(axeCount); i++) {
			if (joystick.axes[i] != axes[i]) {
				joystick.axes[i] = axes[i];
				onAxis((JoystickPort)port, i, joystick.axes[i]);
			}
		}

		int32_t buttonCount = 0;
		auto buttons = glfwGetJoystickButtons(port, &buttonCount);
		joystick.buttons.resize(static_cast<std::size_t>(buttonCount));

		for (uint32_t i = 0; i < static_cast<uint32_t>(buttonCount); i++) {
			if (buttons[i] != GLFW_RELEASE && joystick.buttons[i] != InputAction::Release) {
				joystick.buttons[i] = InputAction::Repeat;
			} else if (joystick.buttons[i] != static_cast<InputAction>(buttons[i])) {
				joystick.buttons[i] = static_cast<InputAction>(buttons[i]);
				onButton((JoystickPort)port, i, joystick.buttons[i]);
			}
		}

		int32_t hatCount = 0;
		auto hats = glfwGetJoystickHats(port, &hatCount);
		joystick.hats.resize(static_cast<std::size_t>(hatCount));

		for (uint32_t i = 0; i < static_cast<uint32_t>(hatCount); i++) {
			if (joystick.hats[i] != bitmask::bitmask<JoystickHatValue>(hats[i])) {
				joystick.hats[i] = bitmask::bitmask<JoystickHatValue>(hats[i]);
				onHat((JoystickPort)port, i, joystick.hats[i]);
			}
		}
	}
}

bool Joysticks::IsConnected(JoystickPort port) const {
	return GetJoystick(port).has_value();
}

std::string Joysticks::GetName(JoystickPort port) const {
	if (auto joystick = GetJoystick(port))
		return joystick->name;
	return "";
}

std::size_t Joysticks::GetAxisCount(JoystickPort port) const {
	if (auto joystick = GetJoystick(port))
		return static_cast<std::size_t>(joystick->axes.size());
	return 0;
}

std::size_t Joysticks::GetButtonCount(JoystickPort port) const {
	if (auto joystick = GetJoystick(port))
		return static_cast<std::size_t>(joystick->buttons.size());
	return 0;
}

std::size_t Joysticks::GetHatCount(JoystickPort port) const {
	if (auto joystick = GetJoystick(port))
		return static_cast<std::size_t>(joystick->hats.size());
	return 0;
}

float Joysticks::GetAxis(JoystickPort port, JoystickAxis axis) const {
	if (auto joystick = GetJoystick(port); joystick && axis <= joystick->axes.size())
		return joystick->axes[axis];
	return 0.0f;
}

InputAction Joysticks::GetButton(JoystickPort port, JoystickButton button) const {
	if (auto joystick = GetJoystick(port); joystick && button <= joystick->buttons.size())
		return joystick->buttons[button];
	return InputAction::Release;
}

bitmask::bitmask<JoystickHatValue> Joysticks::GetHat(JoystickPort port, JoystickHat hat) const {
	if (auto joystick = GetJoystick(port); joystick && hat <= joystick->hats.size())
		return joystick->hats[hat];
	return JoystickHatValue::Centered;
}

std::optional<Joysticks::Joystick> Joysticks::GetJoystick(JoystickPort port) const {
	if (auto joystick = joysticks[(std::size_t)port]; joystick.connected)
		return joystick;
	return std::nullopt;
}
}
