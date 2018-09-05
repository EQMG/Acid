#include "UiSelector.hpp"

namespace acid
{
	UiSelector::UiSelector() :
		m_cursorX(0.0f),
		m_cursorY(0.0f),
		m_selectorMice(std::array<UiSelectorMouse, MOUSE_BUTTON_END_RANGE>())
	{
		for (int32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
		{
			UiSelectorMouse selectorMouse = UiSelectorMouse();
			selectorMouse.m_mouseButton = static_cast<MouseButton>(i);
			selectorMouse.m_isDown = false;
			selectorMouse.m_wasDown = false;
			m_selectorMice[i] = selectorMouse;
		}
	}

	UiSelector::~UiSelector()
	{
	}

	void UiSelector::Update(const bool &paused, const SelectorJoystick &selectorJoystick)
	{
		float delta = Engine::Get()->GetDelta();

		m_cursorX = Mouse::Get()->GetPositionX();
		m_cursorY = Mouse::Get()->GetPositionY();

		if (Joysticks::Get()->IsConnected(selectorJoystick.GetJoystick()))
		{
			if (paused && (std::fabs(Maths::Deadband(0.1f, selectorJoystick.GetAxisX().GetAmount())) > 0.0f ||
				std::fabs(Maths::Deadband(0.1f, selectorJoystick.GetAxisY().GetAmount())) > 0.0f))
			{
				m_cursorX += selectorJoystick.GetAxisX().GetAmount() * 0.75f * delta;
				m_cursorY += selectorJoystick.GetAxisY().GetAmount() * 0.75f * delta;
				m_cursorX = std::clamp(m_cursorX, 0.0f, 1.0f);
				m_cursorY = std::clamp(m_cursorY, 0.0f, 1.0f);
				Mouse::Get()->SetPosition(m_cursorX, m_cursorY);
			}

			for (int32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
			{
				auto buttonJoystick = selectorJoystick.GetInputButton(m_selectorMice[i].m_mouseButton);
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
		float width = object.GetScreenTransform().m_x / 2.0f;
		float height = object.GetScreenTransform().m_y / 2.0f;
		float positionX = (object.GetScreenTransform().m_z + 1.0f) / 2.0f;
		float positionY = (object.GetScreenTransform().m_w - 1.0f) / -2.0f;

		if (Mouse::Get()->IsDisplaySelected() && Display::Get()->IsFocused())
		{
			if (m_cursorX >= positionX && m_cursorX <= positionX + width && 1.0f - m_cursorY >= positionY - height && 1.0f - m_cursorY <= positionY)
			{
				return true;
			}
		}

		return false;
	}

	void UiSelector::CancelWasEvent()
	{
		for (int32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
		{
			m_selectorMice[i].m_wasDown = false;
		}
	}
}
