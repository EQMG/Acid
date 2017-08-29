#include "mouse.hpp"

namespace flounder
{
	void callbackScroll(GLFWwindow *window, double xoffset, double yoffset)
	{
		mouse::get()->m_mouseDeltaWheel = static_cast<float>(yoffset);
	}

	void callbackMouseButton(GLFWwindow *window, int button, int action, int mods)
	{
		mouse::get()->m_mouseButtons[button] = action;
	}

	void callbackCursorPos(GLFWwindow *window, double xpos, double ypos)
	{
		mouse::get()->m_mousePositionX = static_cast<float>(xpos) / static_cast<float>(display::get()->getWidth());
		mouse::get()->m_mousePositionY = static_cast<float>(ypos) / static_cast<float>(display::get()->getHeight());
	}

	void callbackCursorEnter(GLFWwindow *window, int entered)
	{
		mouse::get()->m_displaySelected = entered;
	}

	mouse::mouse() :
		imodule()
	{
		m_customMouse = "";

		m_mouseButtons = new int[GLFW_MOUSE_BUTTON_LAST];
		m_lastMousePositionX = 0.5f;
		m_lastMousePositionY = 0.5f;
		m_mousePositionX = 0.5f;
		m_mousePositionY = 0.5f;
		m_mouseDeltaX = 0.0f;
		m_mouseDeltaY = 0.0f;
		m_mouseDeltaWheel = 0.0f;
		m_displaySelected = true;

		m_cursorDisabled = false;
		m_lastCursorDisabled = false;

		// Sets the default state of the buttons to released.
		for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; i++)
		{
			m_mouseButtons[i] = GLFW_RELEASE;
		}

		// Sets the mouses callbacks.
		glfwSetScrollCallback(display::get()->getWindow(), callbackScroll);
		glfwSetMouseButtonCallback(display::get()->getWindow(), callbackMouseButton);
		glfwSetCursorPosCallback(display::get()->getWindow(), callbackCursorPos);
		glfwSetCursorEnterCallback(display::get()->getWindow(), callbackCursorEnter);
	}

	mouse::~mouse()
	{
		delete m_mouseButtons;
	}

	void mouse::update()
	{
		// Updates the mouses delta.
		m_mouseDeltaX = framework::get()->getDelta() * (m_lastMousePositionX - m_mousePositionX);
		m_mouseDeltaY = framework::get()->getDelta() * (m_lastMousePositionY - m_mousePositionY);

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
		if (m_mouseDeltaWheel != 0.0)
		{
			m_mouseDeltaWheel -= framework::get()->getDelta() * ((m_mouseDeltaWheel < 0.0f) ? -1.0f : 1.0f);
			m_mouseDeltaWheel = maths::deadband(0.1f, m_mouseDeltaWheel);
		}
	}

	void mouse::setCustomMouse(const std::string &customMouse)
	{
		// Loads a custom cursor.
		m_customMouse = customMouse;

		if (!m_customMouse.empty())
		{
			int width = 0;
			int height = 0;
			int components = 0;
			stbi_uc *data = stbi_load(m_customMouse.c_str(), &width, &height, &components, 4);

			if (data == nullptr)
			{
				printf("Unable to load texture: '%s'.\n", m_customMouse.c_str());
			}

			GLFWimage *image = new GLFWimage();
			image->pixels = data;
			image->width = width;
			image->height = height;

			GLFWcursor *cursor = glfwCreateCursor(image, 0, 0);
			glfwSetCursor(display::get()->getWindow(), cursor);
			stbi_image_free(data);
		}
	}

	void mouse::setCursorHidden(const bool &disabled)
	{
		if (m_cursorDisabled != disabled)
		{
			glfwSetInputMode(display::get()->getWindow(), GLFW_CURSOR, (disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));

			if (!disabled && m_cursorDisabled)
			{
				glfwSetCursorPos(display::get()->getWindow(), m_mousePositionX * display::get()->getWidth(), m_mousePositionY * display::get()->getHeight());
			}
		}

		m_cursorDisabled = disabled;
	}

	bool mouse::getButton(const int &button) const
	{
		if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST + 1)
		{
			return false;
		}

		return m_mouseButtons[button] != GLFW_RELEASE;
	}

	void mouse::setPosition(const float &cursorX, const float &cursorY)
	{
		glfwSetCursorPos(display::get()->getWindow(), static_cast<double>(cursorX), static_cast<double>(cursorY));
	}
}
