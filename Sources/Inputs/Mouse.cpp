#include "Mouse.hpp"

#include <GLFW/glfw3.h>
#include "Files/Files.hpp"
#include "Maths/Maths.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset)
	{
		Mouse::Get()->m_mouseDeltaWheel = static_cast<float>(yoffset);
	}

	void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
	{
		Mouse::Get()->m_mouseButtons[button] = action != GLFW_RELEASE;
	}

	void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos)
	{
		Mouse::Get()->m_mousePositionX = static_cast<float>(xpos) / static_cast<float>(Display::Get()->GetWidth());
		Mouse::Get()->m_mousePositionY = (static_cast<float>(ypos) / static_cast<float>(Display::Get()->GetHeight()));
	}

	void CallbackCursorEnter(GLFWwindow *window, int32_t entered)
	{
		Mouse::Get()->m_displaySelected = static_cast<bool>(entered);
	}

	Mouse::Mouse() :
		m_mouseButtons(std::array<bool, MOUSE_BUTTON_END_RANGE>()),
		m_mousePath(""),
		m_lastMousePositionX(0.0f),
		m_lastMousePositionY(0.0f),
		m_mousePositionX(0.0f),
		m_mousePositionY(0.0f),
		m_mouseDeltaX(0.0f),
		m_mouseDeltaY(0.0f),
		m_mouseDeltaWheel(0.0f),
		m_displaySelected(true),
		m_lastCursorDisabled(false),
		m_cursorDisabled(false)
	{
		// Sets the default state of the buttons to released.
		for (uint32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
		{
			m_mouseButtons[i] = false;
		}

		// Sets the mouses callbacks.
		glfwSetScrollCallback(Display::Get()->GetWindow(), CallbackScroll);
		glfwSetMouseButtonCallback(Display::Get()->GetWindow(), CallbackMouseButton);
		glfwSetCursorPosCallback(Display::Get()->GetWindow(), CallbackCursorPos);
		glfwSetCursorEnterCallback(Display::Get()->GetWindow(), CallbackCursorEnter);
	}

	void Mouse::Update()
	{
		float delta = Engine::Get()->GetDelta().AsSeconds();

		// Updates the mouses delta.
		m_mouseDeltaX = delta * (m_lastMousePositionX - m_mousePositionX);
		m_mouseDeltaY = delta * (m_lastMousePositionY - m_mousePositionY);

		// Sets the last position of the current.
		m_lastMousePositionX = m_mousePositionX;
		m_lastMousePositionY = m_mousePositionY;

		// Fixes snaps when toggling cursor.
		if (m_cursorDisabled != m_lastCursorDisabled)
		{
			m_mouseDeltaX = 0.0;
			m_mouseDeltaY = 0.0;

			m_lastCursorDisabled = m_cursorDisabled;
		}

		// Updates the mouse wheel using a smooth scroll technique.
		if (m_mouseDeltaWheel != 0.0f)
		{
			m_mouseDeltaWheel -= delta * ((m_mouseDeltaWheel < 0.0f) ? -3.0f : 3.0f);
			m_mouseDeltaWheel = Maths::Deadband(0.08f, m_mouseDeltaWheel);
		}
	}

	void Mouse::SetCustomMouse(const std::string &filename)
	{
		m_mousePath = filename;

		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t components = 0;
		uint8_t *data = Texture::LoadPixels(m_mousePath, &width, &height, &components);

		if (data == nullptr)
		{
			return;
		}

		GLFWimage image[1];
		image[0].pixels = data;
		image[0].width = width;
		image[0].height = height;

		GLFWcursor *cursor = glfwCreateCursor(image, 0, 0);
		glfwSetCursor(Display::Get()->GetWindow(), cursor);
		Texture::DeletePixels(data);
	}

	void Mouse::SetCursorHidden(const bool &disabled)
	{
		if (m_cursorDisabled != disabled)
		{
			glfwSetInputMode(Display::Get()->GetWindow(), GLFW_CURSOR, (disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));

			if (!disabled && m_cursorDisabled)
			{
				glfwSetCursorPos(Display::Get()->GetWindow(), m_mousePositionX * Display::Get()->GetWidth(), m_mousePositionY * Display::Get()->GetHeight());
			}
		}

		m_cursorDisabled = disabled;
	}

	bool Mouse::GetButton(const MouseButton &mouseButton) const
	{
		if (mouseButton < 0 || mouseButton >= MOUSE_BUTTON_END_RANGE)
		{
			return false;
		}

		return m_mouseButtons[mouseButton];
	}

	void Mouse::SetPosition(const float &cursorX, const float &cursorY)
	{
		m_mousePositionX = cursorX;
		m_mousePositionY = cursorY;
		glfwSetCursorPos(Display::Get()->GetWindow(), cursorX * Display::Get()->GetWidth(), cursorY * Display::Get()->GetHeight());
	}

	void Mouse::SetPosition(const Vector2 &position)
	{
		SetPosition(position.m_x == -1.0f ? GetPositionX() : static_cast<uint32_t>(position.m_x),
		              position.m_y == -1.0f ? GetPositionY() : static_cast<uint32_t>(position.m_y));
	}
}
