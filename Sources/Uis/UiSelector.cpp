#include "UiSelector.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
	UiSelector::UiSelector() :
		m_cursorX(0.0f),
		m_cursorY(0.0f),
		m_selectorMice(std::array<SelectorMouse, MOUSE_BUTTON_END_RANGE>())
	{
		for (int32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
		{
			SelectorMouse selectorMouse = {};
			selectorMouse.m_mouseButton = static_cast<MouseButton>(i);
			selectorMouse.m_isDown = false;
			selectorMouse.m_wasDown = false;
			m_selectorMice[i] = selectorMouse;
		}
	}

	void UiSelector::Update(const VirtualJoystick *virtualJoystick)
	{
		float delta = Engine::Get()->GetDelta().AsSeconds();

		m_cursorX = Mouse::Get()->GetPositionX();
		m_cursorY = Mouse::Get()->GetPositionY();

		if (virtualJoystick != nullptr && Joysticks::Get()->IsConnected(virtualJoystick->GetPort()))
		{
			if (std::fabs(Maths::Deadband(0.1f, virtualJoystick->GetAxisX().GetAmount())) > 0.0f ||
				std::fabs(Maths::Deadband(0.1f, virtualJoystick->GetAxisY().GetAmount())) > 0.0f)
			{
				m_cursorX += virtualJoystick->GetAxisX().GetAmount() * 0.75f * delta;
				m_cursorY += virtualJoystick->GetAxisY().GetAmount() * 0.75f * delta;
				m_cursorX = std::clamp(m_cursorX, 0.0f, 1.0f);
				m_cursorY = std::clamp(m_cursorY, 0.0f, 1.0f);
				Mouse::Get()->SetPosition(m_cursorX, m_cursorY);
			}

			for (int32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
			{
				auto buttonJoystick = virtualJoystick->GetInputButton(m_selectorMice[i].m_mouseButton);
				bool isDown = Mouse::Get()->GetButton(m_selectorMice[i].m_mouseButton) || (buttonJoystick ? buttonJoystick->IsDown() : false);

				if (i == MOUSE_BUTTON_LEFT)
				{
					m_selectorMice[i].m_wasDown = !m_selectorMice[i].m_isDown && isDown;
					m_selectorMice[i].m_isDown = isDown;
				}
			}
		}
		else
		{
			for (int32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
			{
				bool isDown = Mouse::Get()->GetButton(m_selectorMice[i].m_mouseButton);
				m_selectorMice[i].m_wasDown = !m_selectorMice[i].m_isDown && isDown;
				m_selectorMice[i].m_isDown = isDown;
			}
		}
	}

	bool UiSelector::IsSelected(const UiObject &object) const
	{
		Vector2 dimension = object.GetScreenDimension();
		Vector2 position = object.GetScreenPosition();

		if (Mouse::Get()->IsWindowSelected() && Window::Get()->IsFocused())
		{
			if (m_cursorX >= position.m_x && m_cursorX <= position.m_x + dimension.m_x &&
				m_cursorY >= position.m_y && m_cursorY <= position.m_y + dimension.m_y)
			{
				return true;
			}
		}

		return false;
	}

	void UiSelector::CancelWasEvent(const MouseButton &button)
	{
		if (button > MOUSE_BUTTON_END_RANGE)
		{
			return;
		}

		m_selectorMice[static_cast<uint32_t>(button)].m_wasDown = false;
	}
}
