#include "Mouse.hpp"

#include "Files/Files.hpp"
#include "Maths/Maths.hpp"
#include "Textures/Texture.hpp"

namespace fl
{
	void CallbackCursorPosition(WsiShell shell, float x, float y)
	{
		Mouse::Get()->m_mousePositionX = x / static_cast<float>(Display::Get()->GetWidth());
		Mouse::Get()->m_mousePositionY = y / static_cast<float>(Display::Get()->GetHeight());
	}

	void CallbackCursorEnter(WsiShell shell, bool entered)
	{
		Mouse::Get()->m_displaySelected = entered;
	}

	void CallbackScroll(WsiShell shell, float x, float y)
	{
		Mouse::Get()->m_mouseDeltaWheel = y;
	}

	void CallbackMouseButton(WsiShell shell, WsiMouse mouseButton, WsiAction action)
	{
		Mouse::Get()->m_mouseButtons[mouseButton] = action;
	}

	Mouse::Mouse() :
		IModule(),
		m_mousePath(""),
		m_mouseButtons(std::array<WsiAction, WSI_MOUSE_LAST>()),
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
		for (int i = 0; i < WSI_MOUSE_LAST; i++)
		{
			m_mouseButtons[i] = WSI_ACTION_RELEASE;
		}

		// Sets the mouses callbacks.
		wsiCmdSetCursorScrollCallback(Display::Get()->GetWsiShell(), CallbackScroll);
		wsiCmdSetMouseButtonCallback(Display::Get()->GetWsiShell(), CallbackMouseButton);
		wsiCmdSetCursorPositionCallback(Display::Get()->GetWsiShell(), CallbackCursorPosition);
		wsiCmdSetCursorEnterCallback(Display::Get()->GetWsiShell(), CallbackCursorEnter);
	}

	Mouse::~Mouse()
	{
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
			m_mouseDeltaX = 0.0f;
			m_mouseDeltaY = 0.0f;

			m_lastCursorDisabled = m_cursorDisabled;
		}

		// Updates the mouse wheel using a smooth scroll technique.
		if (m_mouseDeltaWheel != 0.0f)
		{
			m_mouseDeltaWheel -= Engine::Get()->GetDelta() * ((m_mouseDeltaWheel < 0.0f) ? -1.0f : 1.0f);
			m_mouseDeltaWheel = Maths::Deadband(0.1f, m_mouseDeltaWheel);
		}
	}

	void Mouse::SetCustomMouse(const std::string &filename)
	{
		// Loads a custom cursor.
		m_mousePath = Files::Get()->SearchFile(filename);

		if (m_mousePath.empty())
		{
			return;
		}

		int width = 0;
		int height = 0;
		int components = 0;
		auto data = Texture::LoadPixels(m_mousePath.c_str(), &width, &height, &components);

		if (data == nullptr)
		{
			fprintf(stderr, "Unable to load texture: '%s'.\n", m_mousePath.c_str());
			return;
		}

		WsiIcon icon = {};
		icon.width = width;
		icon.height = height;
		icon.pixels = data;

		wsiCmdSetCursor(Display::Get()->GetWsiShell(), icon);
		Texture::DeletePixels(data);
	}

	void Mouse::SetCursorHidden(const bool &disabled)
	{
		if (m_cursorDisabled != disabled)
		{
			wsiCmdSetCursorMode(Display::Get()->GetWsiShell(), disabled ? WSI_CURSOR_MODE_DISABLED : WSI_CURSOR_MODE_NORMAL);

			if (!disabled && m_cursorDisabled)
			{
				wsiCmdSetCursorPos(Display::Get()->GetWsiShell(), m_mousePositionX * Display::Get()->GetWidth(), m_mousePositionY * Display::Get()->GetHeight());
			}
		}

		m_cursorDisabled = disabled;
	}

	bool Mouse::GetButton(const WsiMouse &mouseButton) const
	{
		if (mouseButton < 0 || mouseButton > WSI_MOUSE_LAST)
		{
			return false;
		}

		return m_mouseButtons[mouseButton] != WSI_ACTION_RELEASE;
	}

	void Mouse::SetPosition(const float &cursorX, const float &cursorY)
	{
		m_mousePositionX = cursorX;
		m_mousePositionY = cursorY;
		wsiCmdSetCursorPos(Display::Get()->GetWsiShell(), cursorX * Display::Get()->GetWidth(), cursorY * Display::Get()->GetHeight());
	}
}
