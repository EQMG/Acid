#include "joysticks.h"

namespace flounder {
	joysticks::joysticks()
	{
		m_connected = new joystick*[GLFW_JOYSTICK_LAST];

		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			m_connected[i] = new joystick();
		}
	}

	joysticks::~joysticks()
	{
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			delete m_connected[i];
		}

		delete m_connected;
	}

	void joysticks::init()
	{
	}

	void joysticks::update()
	{
		// For each joystick check if connected and update.
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++) 
		{
			joystick* joy = m_connected[i];
			joy->id = i;

			if (glfwJoystickPresent(i)) 
			{
				joy->connected = true;
				joy->name = glfwGetJoystickName(i);
				joy->axes = glfwGetJoystickAxes(i, &joy->axecount);
				joy->buttons = glfwGetJoystickButtons(i, &joy->buttoncount);
			}
			else 
			{
				joy->connected = false;
			}
		}
	}

	bool joysticks::isConnected(int id)
	{
		return m_connected[id]->connected;
	}

	const char *joysticks::getName(int id)
	{
		return m_connected[id]->name;
	}

	float joysticks::getAxis(int id, int axis)
	{
		return m_connected[id]->axes[axis];
	}

	bool joysticks::getButton(int id, int button)
	{
		return m_connected[id]->buttons[button];
	}

	int joysticks::getCountAxes(int id)
	{
		return m_connected[id]->axecount;
	}

	int joysticks::getCountButtons(int id)
	{
		return m_connected[id]->buttoncount;
	}
}
