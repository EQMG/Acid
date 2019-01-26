#include "Mouse.hpp"

#include <GLFW/glfw3.h>
#include "Maths/Maths.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
	{
		Mouse::Get()->m_onButton(static_cast<MouseButton>(button), static_cast<InputAction>(action), make_bitmask<InputMod>(mods));
	}

	void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos)
	{
		Mouse::Get()->m_mousePositionX = static_cast<float>(xpos) / static_cast<float>(Window::Get()->GetWidth());
		Mouse::Get()->m_mousePositionY = (static_cast<float>(ypos) / static_cast<float>(Window::Get()->GetHeight()));
	}

	void CallbackCursorEnter(GLFWwindow *window, int32_t entered)
	{
		Mouse::Get()->m_windowSelected = entered == GLFW_TRUE;
		Mouse::Get()->m_onEnter(entered == GLFW_TRUE);
	}

	void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset)
	{
		Mouse::Get()->m_mouseDeltaWheel = static_cast<float>(yoffset);
	}

	void CallbackDrop(GLFWwindow *window, int32_t count, const char **paths)
	{
		std::vector<std::string> files(static_cast<uint32_t>(count));

		for (uint32_t i = 0; i < count; i++)
		{
			files[i] = paths[i];
		}

		Mouse::Get()->m_onDrop(files);
	}

	Mouse::Mouse() :
		m_lastMousePositionX(0.0f),
		m_lastMousePositionY(0.0f),
		m_mousePositionX(0.0f),
		m_mousePositionY(0.0f),
		m_mouseDeltaX(0.0f),
		m_mouseDeltaY(0.0f),
		m_mouseDeltaWheel(0.0f),
		m_windowSelected(true),
		m_cursorHidden(false),
		m_onButton(Delegate<void(MouseButton, InputAction, bitmask<InputMod>)>()),
		m_onEnter(Delegate<void(bool)>()),
		m_onDrop(Delegate<void(std::vector<std::string>)>())
	{
		glfwSetMouseButtonCallback(Window::Get()->GetWindow(), CallbackMouseButton);
		glfwSetCursorPosCallback(Window::Get()->GetWindow(), CallbackCursorPos);
		glfwSetCursorEnterCallback(Window::Get()->GetWindow(), CallbackCursorEnter);
		glfwSetScrollCallback(Window::Get()->GetWindow(), CallbackScroll);
		glfwSetDropCallback(Window::Get()->GetWindow(), CallbackDrop);
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

		// Updates the mouse wheel using a smooth scroll technique.
		if (m_mouseDeltaWheel != 0.0f)
		{
			m_mouseDeltaWheel -= delta * ((m_mouseDeltaWheel < 0.0f) ? -3.0f : 3.0f);
			m_mouseDeltaWheel = Maths::Deadband(0.08f, m_mouseDeltaWheel);
		}
	}

	void Mouse::SetCursor(const std::string &filename, const CursorHotspot &hotspot)
	{
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t components = 0;
		uint8_t *data = Texture::LoadPixels(filename, &width, &height, &components);

		if (data == nullptr)
		{
			return;
		}

		GLFWimage image[1];
		image[0].width = width;
		image[0].height = height;
		image[0].pixels = data;

		GLFWcursor *cursor = nullptr;

		switch (hotspot)
		{
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
		Texture::DeletePixels(data);
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

	void Mouse::SetPosition(const float &cursorX, const float &cursorY)
	{
		m_mousePositionX = cursorX;
		m_mousePositionY = cursorY;
		glfwSetCursorPos(Window::Get()->GetWindow(), cursorX * Window::Get()->GetWidth(), cursorY * Window::Get()->GetHeight());
	}

	void Mouse::SetPosition(const Vector2 &position)
	{
		SetPosition(position.m_x == -1.0f ? GetPositionX() : static_cast<uint32_t>(position.m_x),
			position.m_y == -1.0f ? GetPositionY() : static_cast<uint32_t>(position.m_y));
	}

	void Mouse::SetCursorHidden(const bool &hidden)
	{
		if (m_cursorHidden != hidden)
		{
			glfwSetInputMode(Window::Get()->GetWindow(), GLFW_CURSOR, hidden ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

			if (!hidden && m_cursorHidden)
			{
				glfwSetCursorPos(Window::Get()->GetWindow(), m_mousePositionX * Window::Get()->GetWidth(), m_mousePositionY * Window::Get()->GetHeight());
			}
		}

		m_cursorHidden = hidden;
	}
}
