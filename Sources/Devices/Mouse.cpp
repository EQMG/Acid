#include "Mouse.hpp"

#include <GLFW/glfw3.h>
#include "Maths/Maths.hpp"
#include "Graphics/Images/Image.hpp"

namespace acid
{
void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
{
	Mouse::Get()->m_onButton(static_cast<MouseButton>(button), static_cast<InputAction>(action), MakeBitMask<InputMod>(mods));
}

void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos)
{
	Mouse::Get()->m_mousePosition.m_x = static_cast<float>(xpos) / static_cast<float>(Window::Get()->GetSize().m_x);
	Mouse::Get()->m_mousePosition.m_y = static_cast<float>(ypos) / static_cast<float>(Window::Get()->GetSize().m_y);
	Mouse::Get()->m_onPosition(Mouse::Get()->m_mousePosition);
}

void CallbackCursorEnter(GLFWwindow *window, int32_t entered)
{
	Mouse::Get()->m_windowSelected = entered == GLFW_TRUE;
	Mouse::Get()->m_onEnter(entered == GLFW_TRUE);
}

void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset)
{
	Mouse::Get()->m_mouseWheelDelta.m_x = static_cast<float>(yoffset);
	Mouse::Get()->m_mouseWheelDelta.m_y = static_cast<float>(yoffset);
	Mouse::Get()->m_onScroll(Mouse::Get()->m_mouseWheelDelta);
}

void CallbackDrop(GLFWwindow *window, int32_t count, const char **paths)
{
	std::vector<std::string> files(static_cast<uint32_t>(count));

	for (uint32_t i = 0; i < static_cast<uint32_t>(count); i++)
	{
		files[i] = paths[i];
	}

	Mouse::Get()->m_onDrop(files);
}

Mouse::Mouse() :
	m_cursor(nullptr),
	m_windowSelected(true),
	m_cursorHidden(false)
{
	glfwSetMouseButtonCallback(Window::Get()->GetWindow(), CallbackMouseButton);
	glfwSetCursorPosCallback(Window::Get()->GetWindow(), CallbackCursorPos);
	glfwSetCursorEnterCallback(Window::Get()->GetWindow(), CallbackCursorEnter);
	glfwSetScrollCallback(Window::Get()->GetWindow(), CallbackScroll);
	glfwSetDropCallback(Window::Get()->GetWindow(), CallbackDrop);
}

Mouse::~Mouse()
{
	glfwDestroyCursor(m_cursor);
}

void Mouse::Update()
{
	float delta = Engine::Get()->GetDelta().AsSeconds();

	// Updates the mouses delta.
	m_mouseDelta = delta * (m_lastMousePosition - m_mousePosition);

	// Sets the last position of the current.
	m_lastMousePosition = m_mousePosition;

	// Updates the mouse wheel using a smooth scroll technique, this is needed because scroll wheel callbacks are not called when scroll is stopped.
	m_mouseWheelDelta.m_x = SmoothScrollWheel(m_mouseWheelDelta.m_x, delta);
	m_mouseWheelDelta.m_y = SmoothScrollWheel(m_mouseWheelDelta.m_y, delta);
}

void Mouse::SetCursor(const std::string &filename, const CursorHotspot &hotspot)
{
	if (m_currentCursor && m_currentCursor->first == filename && m_currentCursor->second == hotspot)
	{
		return;
	}

	Vector2ui extent;
	uint32_t components;
	VkFormat format;
	auto data = Image::LoadPixels(filename, extent, components, format);

	if (data == nullptr)
	{
		return;
	}

	GLFWimage image[1];
	image[0].width = extent.m_x;
	image[0].height = extent.m_y;
	image[0].pixels = data.get();

	glfwDestroyCursor(m_cursor);

	switch (hotspot)
	{
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
	m_currentCursor = std::pair<std::string, CursorHotspot>(filename, hotspot);
	m_currentStandard = {};
}

void Mouse::SetCursor(const CursorStandard &standard)
{
	if (m_currentStandard == standard)
	{
		return;
	}

	glfwDestroyCursor(m_cursor);

	m_cursor = glfwCreateStandardCursor(static_cast<int32_t>(standard));

	glfwSetCursor(Window::Get()->GetWindow(), m_cursor);
	m_currentCursor = {};
	m_currentStandard = standard;
}

std::string Mouse::GetClipboard() const
{
	return glfwGetClipboardString(Window::Get()->GetWindow());
}

void Mouse::SetClipboard(const std::string &string) const
{
	glfwSetClipboardString(Window::Get()->GetWindow(), string.c_str());
}

InputAction Mouse::GetButton(const MouseButton &mouseButton) const
{
	auto state = glfwGetMouseButton(Window::Get()->GetWindow(), static_cast<int32_t>(mouseButton));
	return static_cast<InputAction>(state);
}

void Mouse::SetPosition(const Vector2f &position)
{
	m_mousePosition = position;
	glfwSetCursorPos(Window::Get()->GetWindow(), m_mousePosition.m_x * Window::Get()->GetSize().m_x, m_mousePosition.m_y * Window::Get()->GetSize().m_y);
}

void Mouse::SetCursorHidden(const bool &hidden)
{
	if (m_cursorHidden != hidden)
	{
		glfwSetInputMode(Window::Get()->GetWindow(), GLFW_CURSOR, hidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

		if (!hidden && m_cursorHidden)
		{
			glfwSetCursorPos(Window::Get()->GetWindow(), m_mousePosition.m_x * Window::Get()->GetSize().m_x, m_mousePosition.m_x * Window::Get()->GetSize().m_y);
		}
	}

	m_cursorHidden = hidden;
}

float Mouse::SmoothScrollWheel(float value, const float &delta)
{
	if (value != 0.0f)
	{
		value -= delta * ((value < 0.0f) ? -3.0f : 3.0f);
		value = Maths::Deadband(0.08f, value);
		return value;
	}

	return 0.0f;
}
}
