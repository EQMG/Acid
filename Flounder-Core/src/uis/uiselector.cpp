#include "uiselector.hpp"

namespace flounder
{
	uiselector::uiselector()
	{
		m_cursorX = 0.0f;
		m_cursorY = 0.0f;
		m_leftClick = false;
		m_rightClick = false;

		m_mouseLeft = new buttonmouse(1, GLFW_MOUSE_BUTTON_LEFT);
		m_mouseRight = new buttonmouse(1, GLFW_MOUSE_BUTTON_RIGHT);

		m_joysticksInitialized = false;
	}

	uiselector::~uiselector()
	{
		delete m_mouseLeft;
		delete m_mouseRight;

		delete m_joystickAxisX;
		delete m_joystickAxisY;
		delete m_joystickLeft;
		delete m_joystickRight;
	}

	void uiselector::load(const int &joystick, const int &joystickLeftClick, const int &joystickRightClick, const int &joystickAxisX, const int &joystickAxisY)
	{
		m_selectedJoystick = joystick;
		m_joystickAxisX = new axisjoystick(joystick, 1, joystickAxisX);
		m_joystickAxisY = new axisjoystick(joystick, 1, joystickAxisY);
		m_joystickLeft = new buttonjoystick(joystick, 1, joystickLeftClick);
		m_joystickRight = new buttonjoystick(joystick, 1, joystickRightClick);
		m_joysticksInitialized = true;
	}

	void uiselector::update(const bool &paused)
	{
		m_leftClick = m_mouseLeft->isDown();
		m_rightClick = m_mouseRight->isDown();
		m_leftWasClick = m_mouseLeft->wasDown();
		m_rightWasClick = m_mouseRight->wasDown();

		m_cursorX = mouse::get()->getPositionX();
		m_cursorY = mouse::get()->getPositionY();

		if (m_joysticksInitialized && joysticks::get()->isConnected(m_selectedJoystick) && paused)
		{
			if (fabs(maths::deadband(0.1f, m_joystickAxisX->getAmount())) > 0.0 || fabs(maths::deadband(0.1f, m_joystickAxisY->getAmount())) > 0.0)
			{
				m_cursorX += m_joystickAxisX->getAmount() * 0.75f * framework::get()->getDelta();
				m_cursorY += -m_joystickAxisY->getAmount() * 0.75f * framework::get()->getDelta();
				m_cursorX = maths::clamp(m_cursorX, 0.0f, 1.0f);
				m_cursorY = maths::clamp(m_cursorY, 0.0f, 1.0f);
				mouse::get()->setPosition(m_cursorX * display::get()->getWidth(), m_cursorY * display::get()->getHeight());
			}

			m_leftClick = m_leftClick || m_joystickLeft->isDown();
			m_rightClick = m_rightClick || m_joystickRight->isDown();
			m_leftWasClick = m_leftWasClick || m_joystickLeft->wasDown();
			m_rightWasClick = m_rightWasClick || m_joystickRight->wasDown();
		}
	}

	bool uiselector::isSelected(const uiobject &object)
	{
		// TODO: Account for rotations.
		float positionX = object.getPosition()->m_x;
		float positionY = object.getPosition()->m_y;

		float width = 2.0f * object.getMeshSize()->m_x * object.getScreenDimensions()->m_x / static_cast<float>(display::get()->getAspectRatio());
		float height = 2.0f * object.getMeshSize()->m_y * object.getScreenDimensions()->m_y;

		if (mouse::get()->isDisplaySelected() && display::get()->isFocused())
		{
			if (m_cursorX >= positionX - (width / 2.0f) && m_cursorX <= positionX + (width / 2.0f))
			{
				if (m_cursorY >= positionY - (height / 2.0f) && m_cursorY <= positionY + (height / 2.0f))
				{
					return true;
				}
			}
		}

		return false;
	}

	void uiselector::cancelWasEvent()
	{
		m_leftWasClick = false;
		m_rightWasClick = false;
	}
}
