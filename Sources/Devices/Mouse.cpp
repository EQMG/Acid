#include "Mouse.hpp"

#if 0
#include <GLFW/glfw3.h>
#include "Bitmaps/Bitmap.hpp"
#include "Maths/Maths.hpp"

namespace acid {
static_assert(GLFW_MOUSE_BUTTON_LAST == static_cast<int16_t>(MouseButton::_8), "GLFW mouse button count does not match our mouse button enum count.");

void CallbackMouseButton(GLFWwindow *glfwWindow, int32_t button, int32_t action, int32_t mods) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	Mouse::Get()->onButton(static_cast<MouseButton>(button), static_cast<InputAction>(action), bitmask::bitmask<InputMod>(mods));
}

void CallbackCursorPos(GLFWwindow *glfwWindow, double xpos, double ypos) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	Mouse::Get()->position = {xpos, ypos};
	Mouse::Get()->onPosition(Mouse::Get()->position);
}

void CallbackScroll(GLFWwindow *glfwWindow, double xoffset, double yoffset) {
	auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
	Mouse::Get()->scroll = {yoffset, yoffset};
	Mouse::Get()->onScroll(Mouse::Get()->scroll);
}

Mouse::Mouse() {
	Windows::Get()->OnAddWindow().connect(this, [this](Window *window, bool added) {
		glfwSetMouseButtonCallback(window->GetWindow(), CallbackMouseButton);
		glfwSetCursorPosCallback(window->GetWindow(), CallbackCursorPos);
		glfwSetScrollCallback(window->GetWindow(), CallbackScroll);
	});
}

Mouse::~Mouse() {
}

void Mouse::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	// Updates the position delta.
	positionDelta = delta * (lastPosition - position);
	lastPosition = position;

	// Updates the scroll delta.
	scrollDelta = delta * (lastScroll - scroll);
	lastScroll = scroll;
}

InputAction Mouse::GetButton(MouseButton mouseButton) const {
	auto state = glfwGetMouseButton(Windows::Get()->GetWindow(0)->GetWindow(), static_cast<int32_t>(mouseButton));
	return static_cast<InputAction>(state);
}

void Mouse::SetPosition(const Vector2d &position) {
	this->lastPosition = position;
	this->position = position;
	glfwSetCursorPos(Windows::Get()->GetWindow(0)->GetWindow(), position.x, position.y);
}

void Mouse::SetScroll(const Vector2d &scroll) {
	this->lastScroll = scroll;
	this->scroll = scroll;
}

double Mouse::SmoothScrollWheel(double value, float delta) {
	if (value != 0.0) {
		value -= static_cast<double>(delta) * std::copysign(3.0, value);
		value = Maths::Deadband(0.08, value);
		return value;
	}

	return 0.0;
}
}
#endif
