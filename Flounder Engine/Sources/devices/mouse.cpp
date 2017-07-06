#include "mouse.h"

namespace flounder {
	mouse::mouse()
	{
		m_mouseButtons = new int[GLFW_MOUSE_BUTTON_LAST];
		m_displaySelected = true;
		m_mousePositionX = 0.5f;
		m_mousePositionY = 0.5f;	
		
		m_cursorDisabled = false;
		m_lastCursorDisabled = false;

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
		if (m_cursorDisabled != m_lastCursorDisabled) {
			m_mouseDeltaX = 0.0f;
			m_mouseDeltaY = 0.0f;

			m_lastCursorDisabled = m_cursorDisabled;
		}

		// Updates the mouse wheel using a smooth scroll technique.
		if (m_mouseDeltaWheel != 0.0f) {
			m_mouseDeltaWheel -= framework::get()->getDelta() * ((m_mouseDeltaWheel < 0.0f) ? -1.0f : 1.0f);
			m_mouseDeltaWheel = maths::deadband(0.1f, m_mouseDeltaWheel);
		}
	}

	void mouse::setCursorHidden(bool disabled)
	{
		if (m_cursorDisabled != disabled) {
			glfwSetInputMode(display::get()->getWindow(), GLFW_CURSOR, (disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));

			if (!disabled && m_cursorDisabled) {
				glfwSetCursorPos(display::get()->getWindow(), m_mousePositionX * display::get()->getWidth(), m_mousePositionY * display::get()->getHeight());
			}
		}

		m_cursorDisabled = disabled;
	}

	bool mouse::getMouse(int button)
	{
		return m_mouseButtons[button] != GLFW_RELEASE;
	}

	float mouse::getPositionX()
	{
		return m_mousePositionX;
	}

	float mouse::getPositionY()
	{
		return m_mousePositionY;
	}

	void mouse::setPosition(float cursorX, float cursorY)
	{
		glfwSetCursorPos(display::get()->getWindow(), cursorX, cursorY);
	}

	float mouse::getDeltaX()
	{
		return m_mouseDeltaX;
	}

	float mouse::getDeltaY()
	{
		return m_mouseDeltaY;
	}

	float mouse::getDeltaWheel()
	{
		return m_mouseDeltaWheel;
	}

	bool mouse::isDisplaySelected()
	{
		return m_displaySelected;
	}

	bool mouse::isCursorDisabled()
	{
		return m_cursorDisabled;
	}

	void callbackScroll(GLFWwindow *window, double xoffset, double yoffset)
	{
		mouse::get()->m_mouseDeltaWheel = (float)yoffset;
	}

	void callbackMouseButton(GLFWwindow *window, int button, int action, int mods)
	{
		mouse::get()->m_mouseButtons[button] = action;
	}

	void callbackCursorPos(GLFWwindow *window, double xpos, double ypos)
	{
		mouse::get()->m_mousePositionX = (float)(xpos / display::get()->getWidth());
		mouse::get()->m_mousePositionY = (float)(ypos / display::get()->getHeight());
	}

	void callbackCursorEnter(GLFWwindow *window, int entered)
	{
		mouse::get()->m_displaySelected = entered;
	}
}
