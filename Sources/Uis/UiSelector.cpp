#include "UiSelector.hpp"

namespace fl
{
	UiSelector::UiSelector() :
		m_cursorX(0.0f),
		m_cursorY(0.0f),
		m_leftClick(false),
		m_rightClick(false),
		m_leftWasClick(false),
		m_rightWasClick(false),
		m_mouseLeft(ButtonMouse({WSI_MOUSE_BUTTON_LEFT})),
		m_mouseRight(ButtonMouse({WSI_MOUSE_BUTTON_RIGHT}))
	{
	}

	UiSelector::~UiSelector()
	{
	}

	void UiSelector::Update(const bool &paused, const SelectorJoystick &selectorJoystick)
	{
		float delta = Engine::Get()->GetDelta();

		m_leftClick = m_mouseLeft.IsDown();
		m_rightClick = m_mouseRight.IsDown();
		m_leftWasClick = m_mouseLeft.WasDown();
		m_rightWasClick = m_mouseRight.WasDown();

		m_cursorX = Mouse::Get()->GetPositionX();
		m_cursorY = Mouse::Get()->GetPositionY();

		if (Joysticks::Get()->IsConnected(selectorJoystick.GetJoystick()) && paused)
		{
			if (std::fabs(Maths::Deadband(0.1f, selectorJoystick.GetAxisX().GetAmount())) > 0.0f ||
				std::fabs(Maths::Deadband(0.1f, selectorJoystick.GetAxisY().GetAmount())) > 0.0f)
			{
				m_cursorX += selectorJoystick.GetAxisX().GetAmount() * 0.75f * delta;
				m_cursorY += selectorJoystick.GetAxisY().GetAmount() * 0.75f * delta;
				m_cursorX = Maths::Clamp(m_cursorX, 0.0f, 1.0f);
				m_cursorY = Maths::Clamp(m_cursorY, 0.0f, 1.0f);
				Mouse::Get()->SetPosition(m_cursorX, m_cursorY);
			}

			m_leftClick = m_leftClick || selectorJoystick.GetClickLeft().IsDown();
			m_rightClick = m_rightClick || selectorJoystick.GetClickRight().IsDown();
			m_leftWasClick = m_leftWasClick || selectorJoystick.GetClickLeft().WasDown();
			m_rightWasClick = m_rightWasClick || selectorJoystick.GetClickRight().WasDown();
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
		m_leftWasClick = false;
		m_rightWasClick = false;
	}
}
