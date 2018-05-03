#include "Mouse.hpp"

#include "Maths/Maths.hpp"
#include "Textures/Texture.hpp"

namespace fl
{
	void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset)
	{
		Mouse::Get()->m_mouseDeltaWheel = static_cast<float>(yoffset);
	}

	void CallbackMouseButton(GLFWwindow *window, int button, int action, int mods)
	{
		Mouse::Get()->m_mouseButtons[button] = action;
	}

	void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos)
	{
		Mouse::Get()->m_mousePositionX = static_cast<float>(xpos) / static_cast<float>(Display::Get()->GetWidth());
		Mouse::Get()->m_mousePositionY = (static_cast<float>(ypos) / static_cast<float>(Display::Get()->GetHeight()));
	}

	void CallbackCursorEnter(GLFWwindow *window, int entered)
	{
		Mouse::Get()->m_displaySelected = static_cast<bool>(entered);
	}

	Mouse::Mouse() :
		IModule(),
		m_customMouse(""),
		m_mouseButtons(new int[GLFW_MOUSE_BUTTON_LAST]),
		m_lastMousePositionX(0.5f),
		m_lastMousePositionY(0.5f),
		m_mousePositionX(0.5f),
		m_mousePositionY(0.5f),
		m_mouseDeltaX(0.0f),
		m_mouseDeltaY(0.0f),
		m_mouseDeltaWheel(0.0f),
		m_displaySelected(true),
		m_cursorDisabled(false),
		m_lastCursorDisabled(false)
	{
		// Sets the default state of the buttons to released.
		for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; i++)
		{
			m_mouseButtons[i] = GLFW_RELEASE;
		}

		// Sets the mouses callbacks.
		glfwSetScrollCallback(Display::Get()->GetWindow(), CallbackScroll);
		glfwSetMouseButtonCallback(Display::Get()->GetWindow(), CallbackMouseButton);
		glfwSetCursorPosCallback(Display::Get()->GetWindow(), CallbackCursorPos);
		glfwSetCursorEnterCallback(Display::Get()->GetWindow(), CallbackCursorEnter);
	}

	Mouse::~Mouse()
	{
		delete[] m_mouseButtons;
	}

	void Mouse::Update()
	{
		// Updates the mouses delta.
		m_mouseDeltaX = Engine::Get()->GetDelta() * (m_lastMousePositionX - m_mousePositionX);
		m_mouseDeltaY = Engine::Get()->GetDelta() * (m_lastMousePositionY - m_mousePositionY);

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
			m_mouseDeltaWheel -= Engine::Get()->GetDelta() * ((m_mouseDeltaWheel < 0.0f) ? -1.0f : 1.0f);
			m_mouseDeltaWheel = Maths::Deadband(0.1f, m_mouseDeltaWheel);
		}
	}

	void Mouse::SetCustomMouse(const std::string &customMouse)
	{
		// Loads a custom cursor.
		m_customMouse = customMouse;

		if (!m_customMouse.empty())
		{
			int width = 0;
			int height = 0;
			int components = 0;
			stbi_uc *data = Texture::LoadPixels(m_customMouse.c_str(), &width, &height, &components);

			if (data == nullptr)
			{
				printf("Unable to load texture: '%s'.\n", m_customMouse.c_str());
			}

			GLFWimage *image = new GLFWimage();
			image->pixels = data;
			image->width = width;
			image->height = height;

			GLFWcursor *cursor = glfwCreateCursor(image, 0, 0);
			glfwSetCursor(Display::Get()->GetWindow(), cursor);
			stbi_image_free(data);
			//	delete image;
		}
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

	bool Mouse::GetButton(const int &button) const
	{
		if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST + 1)
		{
			return false;
		}

		return m_mouseButtons[button] != GLFW_RELEASE;
	}

	void Mouse::SetPosition(const float &cursorX, const float &cursorY)
	{
		m_mousePositionX = cursorX;
		m_mousePositionY = cursorY;
		glfwSetCursorPos(Display::Get()->GetWindow(), cursorX * Display::Get()->GetWidth(), cursorY * Display::Get()->GetHeight());
	}
}
