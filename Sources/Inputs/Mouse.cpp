#include "Mouse.hpp"

#include "Files/Files.hpp"
#include "Maths/Maths.hpp"
#include "Textures/Texture.hpp"

namespace acid
{
	void CallbackCursorPosition(float x, float y, float dx, float dy)
	{
		Mouse::Get()->m_mousePositionX = x;
		Mouse::Get()->m_mousePositionY = y;
		Mouse::Get()->m_mouseDeltaX = dx;
		Mouse::Get()->m_mouseDeltaY = dy;
	}

	void CallbackCursorEnter(VkBool32 entered)
	{
		Mouse::Get()->m_displaySelected = entered;
	}

	void CallbackCursorScroll(float x, float y)
	{
		Mouse::Get()->m_mouseDeltaWheel = y;
	}

	void CallbackMouseButton(MouseButton mouseButton, bool isDown)
	{
		Mouse::Get()->m_mouseButtons[mouseButton] = isDown;
	}

	Mouse::Mouse() :
		IModule(),
		m_mousePath(""),
		m_mouseButtons(std::array<bool, MOUSE_BUTTON_END_RANGE>()),
		m_mousePositionX(0.0f),
		m_mousePositionY(0.0f),
		m_mouseDeltaX(0.0f),
		m_mouseDeltaY(0.0f),
		m_mouseDeltaWheel(0.0f),
		m_displaySelected(true),
		m_cursorDisabled(false)
	{
		auto shell = Display::Get()->GetShell();

		// Sets the default state of the buttons to released.
		for (int i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
		{
			m_mouseButtons[i] = false;
		}

		// Sets the mouses callbacks.
		shell->SetCallbackCursorScroll(CallbackCursorScroll);
		shell->SetCallbackMouseButton(CallbackMouseButton);
		shell->SetCallbackCursorPosition(CallbackCursorPosition);
		shell->SetCallbackCursorEnter(CallbackCursorEnter);
	}

	Mouse::~Mouse()
	{
	}

	void Mouse::Update()
	{
		// Updates the mouse wheel using a smooth scroll technique.
		if (m_mouseDeltaWheel != 0.0f)
		{
			m_mouseDeltaWheel -= Engine::Get()->GetDelta() * ((m_mouseDeltaWheel < 0.0f) ? -1.0f : 1.0f);
			m_mouseDeltaWheel = Maths::Deadband(0.1f, m_mouseDeltaWheel);
		}
	}

	void Mouse::SetCustomMouse(const std::string &filename)
	{
		auto shell = Display::Get()->GetShell();

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

		shell->SetCursorImage(data, width, height);
		Texture::DeletePixels(data);
	}

	void Mouse::SetCursorHidden(const bool &disabled)
	{
		auto shell = Display::Get()->GetShell();

		if (m_cursorDisabled != disabled)
		{
			shell->SetCursorMode(disabled ? CURSOR_MODE_DISABLED : CURSOR_MODE_NORMAL);

			if (!disabled && m_cursorDisabled)
			{
				shell->SetCursorPos(m_mousePositionX * Display::Get()->GetWidth(), m_mousePositionY * Display::Get()->GetHeight());
			}
		}

		m_cursorDisabled = disabled;
	}

	bool Mouse::GetButton(const MouseButton &mouseButton) const
	{
		if (mouseButton < 0 || mouseButton > MOUSE_BUTTON_END_RANGE)
		{
			return false;
		}

		return m_mouseButtons[mouseButton];
	}

	void Mouse::SetPosition(const float &cursorX, const float &cursorY)
	{
		auto shell = Display::Get()->GetShell();

		m_mousePositionX = cursorX;
		m_mousePositionY = cursorY;
		shell->SetCursorPos(cursorX * Display::Get()->GetWidth(), cursorY * Display::Get()->GetHeight());
	}
}
