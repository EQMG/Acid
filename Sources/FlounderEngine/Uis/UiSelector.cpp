#include "UiSelector.hpp"

#include "../Devices/Mouse.hpp"
#include "../Devices/Joysticks.hpp"

namespace Flounder
{
	UiSelector::UiSelector() :
		m_cursorX(0.0f),
		m_cursorY(0.0f),
		m_leftClick(false),
		m_rightClick(false),
		m_leftWasClick(false),
		m_rightWasClick(false),
		m_mouseLeft(new ButtonMouse({GLFW_MOUSE_BUTTON_LEFT})),
		m_mouseRight(new ButtonMouse({GLFW_MOUSE_BUTTON_RIGHT})),
		m_selectorJoystick(nullptr)
	{
	}

	UiSelector::~UiSelector()
	{
		delete m_mouseLeft;
		delete m_mouseRight;

		delete m_selectorJoystick;
	}

	void UiSelector::Load(const unsigned int &joystick, const int &joystickLeftClick, const int &joystickRightClick, const int &joystickAxisX, const int &joystickAxisY)
	{
		m_selectorJoystick = new SelectorJoystick{
			joystick,
			new ButtonJoystick(joystick, {joystickLeftClick}),
			new ButtonJoystick(joystick, {joystickRightClick}),
			new AxisJoystick(joystick, {joystickAxisX}),
			new AxisJoystick(joystick, {joystickAxisY})
		};
	}

	void UiSelector::Update(const bool &paused)
	{
		float delta = Engine::Get()->GetDelta();

		m_leftClick = m_mouseLeft->IsDown();
		m_rightClick = m_mouseRight->IsDown();
		m_leftWasClick = m_mouseLeft->WasDown();
		m_rightWasClick = m_mouseRight->WasDown();

		m_cursorX = Mouse::Get()->GetPositionX();
		m_cursorY = Mouse::Get()->GetPositionY();

		if (m_selectorJoystick != nullptr && Joysticks::Get()->IsConnected(m_selectorJoystick->joystick) && paused)
		{
			if (std::fabs(Maths::Deadband(0.1f, m_selectorJoystick->axisX->GetAmount())) > 0.0f ||
				std::fabs(Maths::Deadband(0.1f, m_selectorJoystick->axisY->GetAmount())) > 0.0f)
			{
				m_cursorX += m_selectorJoystick->axisX->GetAmount() * 0.75f * delta;
				m_cursorY += m_selectorJoystick->axisY->GetAmount() * 0.75f * delta;
				m_cursorX = Maths::Clamp(m_cursorX, 0.0f, 1.0f);
				m_cursorY = Maths::Clamp(m_cursorY, 0.0f, 1.0f);
				Mouse::Get()->SetPosition(m_cursorX, m_cursorY);
			}

			m_leftClick = m_leftClick || m_selectorJoystick->clickLeft->IsDown();
			m_rightClick = m_rightClick || m_selectorJoystick->clickRight->IsDown();
			m_leftWasClick = m_leftWasClick || m_selectorJoystick->clickLeft->WasDown();
			m_rightWasClick = m_rightWasClick || m_selectorJoystick->clickRight->WasDown();
		}
	}

	bool UiSelector::IsSelected(const UiObject &object) const
	{
		const float width = object.GetScreenTransform()->m_x / 2.0f;
		const float height = object.GetScreenTransform()->m_y / 2.0f;
		const float positionX = (object.GetScreenTransform()->m_z + 1.0f) / 2.0f;
		const float positionY = (object.GetScreenTransform()->m_w - 1.0f) / -2.0f;

		if (Mouse::Get()->IsDisplaySelected() && Display::Get()->IsFocused())
		{
			if (m_cursorX >= positionX && m_cursorX <= positionX + width
				&& 1.0f - m_cursorY >= positionY - height && 1.0f - m_cursorY <= positionY)
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
