#include "Mouse.hpp"

#include <GLFW/glfw3.h>
#include "Bitmaps/Bitmap.hpp"
#include "Maths/Maths.hpp"

namespace acid {
static_assert(GLFW_MOUSE_BUTTON_LAST == static_cast<int16_t>(MouseButton::_8), "GLFW mouse button count does not match our mouse button enum count.");

void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
	Mouse::Get()->onButton(static_cast<MouseButton>(button), static_cast<InputAction>(action), bitmask::bitmask<InputMod>(mods));
}

void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos) {
	Mouse::Get()->position = {xpos, ypos};
	Mouse::Get()->onPosition(Mouse::Get()->position);
}

void CallbackCursorEnter(GLFWwindow *window, int32_t entered) {
	Mouse::Get()->windowSelected = entered == GLFW_TRUE;
	Mouse::Get()->onEnter(entered == GLFW_TRUE);
}

void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset) {
	Mouse::Get()->scroll = {yoffset, yoffset};
	Mouse::Get()->onScroll(Mouse::Get()->scroll);
}

void CallbackDrop(GLFWwindow *window, int32_t count, const char **paths) {
	std::vector<std::string> files(static_cast<uint32_t>(count));
	for (uint32_t i = 0; i < static_cast<uint32_t>(count); i++)
		files[i] = paths[i];

	Mouse::Get()->onDrop(files);
}

Mouse::Mouse() {
	glfwSetMouseButtonCallback(Window::Get()->GetWindow(), CallbackMouseButton);
	glfwSetCursorPosCallback(Window::Get()->GetWindow(), CallbackCursorPos);
	glfwSetCursorEnterCallback(Window::Get()->GetWindow(), CallbackCursorEnter);
	glfwSetScrollCallback(Window::Get()->GetWindow(), CallbackScroll);
	glfwSetDropCallback(Window::Get()->GetWindow(), CallbackDrop);
}

Mouse::~Mouse() {
	glfwDestroyCursor(cursor);
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

void Mouse::SetCursor(const std::filesystem::path &filename, CursorHotspot hotspot) {
	if (currentCursor && currentCursor->first == filename && currentCursor->second == hotspot) {
		return;
	}

	Bitmap bitmap(filename);
	if (!bitmap) return;

	GLFWimage image[1];
	image[0].width = bitmap.GetSize().x;
	image[0].height = bitmap.GetSize().y;
	image[0].pixels = bitmap.GetData().get();

	glfwDestroyCursor(cursor);

	switch (hotspot) {
	case CursorHotspot::UpperLeft:
		cursor = glfwCreateCursor(image, 0, 0);
		break;
	case CursorHotspot::UpperRight:
		cursor = glfwCreateCursor(image, image->width - 1, 0);
		break;
	case CursorHotspot::BottomLeft:
		cursor = glfwCreateCursor(image, 0, image->height - 1);
		break;
	case CursorHotspot::BottomRight:
		cursor = glfwCreateCursor(image, image->width - 1, image->height - 1);
		break;
	case CursorHotspot::Centered:
		cursor = glfwCreateCursor(image, image->width / 2, image->height / 2);
		break;
	}

	glfwSetCursor(Window::Get()->GetWindow(), cursor);
	currentCursor = {filename, hotspot};
	currentStandard = std::nullopt;
}

void Mouse::SetCursor(CursorStandard standard) {
	if (currentStandard == standard) return;

	glfwDestroyCursor(cursor);

	cursor = glfwCreateStandardCursor(static_cast<int32_t>(standard));

	glfwSetCursor(Window::Get()->GetWindow(), cursor);
	currentCursor = std::nullopt;
	currentStandard = standard;
}

std::string Mouse::GetClipboard() const {
	return glfwGetClipboardString(Window::Get()->GetWindow());
}

void Mouse::SetClipboard(const std::string &string) const {
	glfwSetClipboardString(Window::Get()->GetWindow(), string.c_str());
}

InputAction Mouse::GetButton(MouseButton mouseButton) const {
	auto state = glfwGetMouseButton(Window::Get()->GetWindow(), static_cast<int32_t>(mouseButton));
	return static_cast<InputAction>(state);
}

void Mouse::SetPosition(const Vector2d &position) {
	this->lastPosition = position;
	this->position = position;
	glfwSetCursorPos(Window::Get()->GetWindow(), position.x, position.y);
}

void Mouse::SetScroll(const Vector2d &scroll) {
	this->lastScroll = scroll;
	this->scroll = scroll;
}

void Mouse::SetCursorHidden(bool hidden) {
	if (cursorHidden != hidden) {
		glfwSetInputMode(Window::Get()->GetWindow(), GLFW_CURSOR, hidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

		if (!hidden && cursorHidden)
			SetPosition(position);
	}

	cursorHidden = hidden;
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
