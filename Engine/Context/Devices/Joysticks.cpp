#include "Joysticks.hpp"

#include <GLFW/glfw3.h>

namespace acid {
static_assert(GLFW_JOYSTICK_LAST == static_cast<int16_t>(JoystickPort::_16), "GLFW joystick port count does not match our joystick port enum count.");

void CallbackJoystick(int32_t id, int32_t event) {
	auto joystick = Joysticks::Get()->GetJoystick(static_cast<JoystickPort>(id));
	if (event == GLFW_CONNECTED) {
		Log::Out("Joystick connected: '", glfwGetJoystickName(id), "' to ", id, '\n');
		joystick->connected = true;
		joystick->name = glfwGetJoystickName(id);
		joystick->onConnect(true);
	} else if (event == GLFW_DISCONNECTED) {
		Log::Out("Joystick disconnected from ", id, '\n');
		joystick->connected = false;
		joystick->onConnect(false);
	}
}

void Joystick::Update() {
	if (!connected) return;

	int32_t axeCount = 0;
	auto axesPtr = glfwGetJoystickAxes(static_cast<int>(port), &axeCount);
	axes.resize(static_cast<std::size_t>(axeCount));

	for (uint32_t i = 0; i < static_cast<uint32_t>(axeCount); i++) {
		if (axes[i] != axesPtr[i]) {
			axes[i] = axesPtr[i];
			onAxis(i, axes[i]);
		}
	}

	int32_t buttonCount = 0;
	auto buttonsPtr = glfwGetJoystickButtons(static_cast<int>(port), &buttonCount);
	buttons.resize(static_cast<std::size_t>(buttonCount));

	for (uint32_t i = 0; i < static_cast<uint32_t>(buttonCount); i++) {
		if (buttonsPtr[i] != GLFW_RELEASE && buttons[i] != InputAction::Release) {
			buttons[i] = InputAction::Repeat;
		} else if (buttons[i] != static_cast<InputAction>(buttonsPtr[i])) {
			buttons[i] = static_cast<InputAction>(buttonsPtr[i]);
			onButton(i, buttons[i]);
		}
	}

	int32_t hatCount = 0;
	auto hatsPtr = glfwGetJoystickHats(static_cast<int>(port), &hatCount);
	hats.resize(static_cast<std::size_t>(hatCount));

	for (uint32_t i = 0; i < static_cast<uint32_t>(hatCount); i++) {
		if (hats[i] != bitmask::bitmask<JoystickHatValue>(hatsPtr[i])) {
			hats[i] = bitmask::bitmask<JoystickHatValue>(hatsPtr[i]);
			onHat(i, hats[i]);
		}
	}
}

float Joystick::GetAxis(JoystickAxis axis) const {
	if (connected && axis <= axes.size())
		return axes[axis];
	return 0.0f;
}

InputAction Joystick::GetButton(JoystickButton button) const {
	if (connected && button <= buttons.size())
		return buttons[button];
	return InputAction::Release;
}

bitmask::bitmask<JoystickHatValue> Joystick::GetHat(JoystickHat hat) const {
	if (connected && hat <= hats.size())
		return hats[hat];
	return JoystickHatValue::Centered;
}

Joysticks::Joysticks() {
	glfwSetJoystickCallback(CallbackJoystick);

	for (auto [port, joystick] : Enumerate(joysticks)) {
		if (glfwJoystickPresent(port)) {
			joystick.port = static_cast<JoystickPort>(port);
			joystick.name = glfwGetJoystickName(port);
			joystick.onConnect(true);
		}
	}
}

void Joysticks::Update() {
	for (auto [port, joystick] : Enumerate(joysticks)) {
		joystick.Update();
	}
}

const Joystick *Joysticks::GetJoystick(JoystickPort port) const {
	return &joysticks[(std::size_t)port];
}

Joystick *Joysticks::GetJoystick(JoystickPort port) {
	return &joysticks[(std::size_t)port];
}
}
