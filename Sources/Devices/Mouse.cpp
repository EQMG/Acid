#include "Mouse.hpp"

#include <GLFW/glfw3.h>
#include "Bitmaps/Bitmap.hpp"
#include "Maths/Maths.hpp"

namespace acid {
void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods) {
	Mouse::Get()->m_onButton(static_cast<MouseButton>(button), static_cast<InputAction>(action), MakeBitMask<InputMod>(mods));
}

void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos) {
	Mouse::Get()->m_position = {xpos, ypos};
	Mouse::Get()->m_onPosition(Mouse::Get()->m_position);
}

void CallbackCursorEnter(GLFWwindow *window, int32_t entered) {
	Mouse::Get()->m_windowSelected = entered == GLFW_TRUE;
	Mouse::Get()->m_onEnter(entered == GLFW_TRUE);
}

void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset) {
	Mouse::Get()->m_scroll = {yoffset, yoffset};
	Mouse::Get()->m_onScroll(Mouse::Get()->m_scroll);
}

void CallbackDrop(GLFWwindow *window, int32_t count, const char **paths) {
	std::vector<std::string> files(static_cast<uint32_t>(count));

	for (uint32_t i = 0; i < static_cast<uint32_t>(count); i++) {
		files[i] = paths[i];
	}

	Mouse::Get()->m_onDrop(files);
}

Mouse::Mouse() {
	glfwSetMouseButtonCallback(Window::Get()->GetWindow(), CallbackMouseButton);
	glfwSetCursorPosCallback(Window::Get()->GetWindow(), CallbackCursorPos);
	glfwSetCursorEnterCallback(Window::Get()->GetWindow(), CallbackCursorEnter);
	glfwSetScrollCallback(Window::Get()->GetWindow(), CallbackScroll);
	glfwSetDropCallback(Window::Get()->GetWindow(), CallbackDrop);
}

Mouse::~Mouse() {
	glfwDestroyCursor(m_cursor);
}

void Mouse::Update() {
	auto delta = Engine::Get()->GetDelta().AsSeconds();

	// Updates the position delta.
	m_positionDelta = delta * (m_lastPosition - m_position);
	m_lastPosition = m_position;

	// Updates the scroll delta.
	m_scrollDelta = delta * (m_lastScroll - m_scroll);
	m_lastScroll = m_scroll;
}

void Mouse::SetCursor(const std::filesystem::path &filename, CursorHotspot hotspot) {
	if (m_currentCursor && m_currentCursor->first == filename && m_currentCursor->second == hotspot) {
		return;
	}

	auto bitmap = Bitmap::Load(filename);

	if (!bitmap) {
		return;
	}

	GLFWimage image[1];
	image[0].width = bitmap->m_size.m_x;
	image[0].height = bitmap->m_size.m_y;
	image[0].pixels = bitmap->m_data.data();

	glfwDestroyCursor(m_cursor);

	switch (hotspot) {
	case CursorHotspot::UpperLeft:
		m_cursor = glfwCreateCursor(image, 0, 0);
		break;
	case CursorHotspot::UpperRight:
		m_cursor = glfwCreateCursor(image, image->width - 1, 0);
		break;
	case CursorHotspot::BottomLeft:
		m_cursor = glfwCreateCursor(image, 0, image->height - 1);
		break;
	case CursorHotspot::BottomRight:
		m_cursor = glfwCreateCursor(image, image->width - 1, image->height - 1);
		break;
	case CursorHotspot::Centered:
		m_cursor = glfwCreateCursor(image, image->width / 2, image->height / 2);
		break;
	}

	glfwSetCursor(Window::Get()->GetWindow(), m_cursor);
	m_currentCursor = {filename, hotspot};
	m_currentStandard = std::nullopt;
}

void Mouse::SetCursor(CursorStandard standard) {
	if (m_currentStandard == standard) {
		return;
	}

	glfwDestroyCursor(m_cursor);

	m_cursor = glfwCreateStandardCursor(static_cast<int32_t>(standard));

	glfwSetCursor(Window::Get()->GetWindow(), m_cursor);
	m_currentCursor = std::nullopt;
	m_currentStandard = standard;
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
	m_lastPosition = position;
	m_position = position;
	glfwSetCursorPos(Window::Get()->GetWindow(), m_position.m_x, m_position.m_y);
}

void Mouse::SetScroll(const Vector2d &scroll) {
	m_lastScroll = scroll;
	m_scroll = scroll;
}

void Mouse::SetCursorHidden(bool hidden) {
	if (m_cursorHidden != hidden) {
		glfwSetInputMode(Window::Get()->GetWindow(), GLFW_CURSOR, hidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

		if (!hidden && m_cursorHidden) {
			SetPosition(m_position);
		}
	}

	m_cursorHidden = hidden;
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
