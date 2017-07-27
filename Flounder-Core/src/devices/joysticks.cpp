#include "joysticks.h"

namespace flounder
{
	joysticks::joysticks() :
		imodule()
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

	void joysticks::update()
	{
		// For each joystick check if connected and update.
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			joystick *joy = m_connected[i];
			joy->id = i;

			if (glfwJoystickPresent(i))
			{
				if (!joy->connected)
				{
					std::cout << "Joystick connected: " << glfwGetJoystickName(i) << std::endl;
				}

				joy->connected = true;
				joy->name = glfwGetJoystickName(i);
				joy->axes = glfwGetJoystickAxes(i, &joy->axecount);
				joy->buttons = glfwGetJoystickButtons(i, &joy->buttoncount);
			}
			else
			{
				if (joy->connected)
				{
					std::cout << "Joystick disconnected!" << std::endl;
				}

				joy->connected = false;
			}
		}
	}

	bool joysticks::isConnected(const int &id)
	{
		return m_connected[id]->connected;
	}

	const char *joysticks::getName(const int &id)
	{
		return m_connected[id]->name;
	}

	float joysticks::getAxis(const int &id, const int &axis)
	{
		return m_connected[id]->axes[axis];
	}

	bool joysticks::getButton(const int &id, const int &button)
	{
		return m_connected[id]->buttons[button];
	}

	int joysticks::getCountAxes(const int &id)
	{
		return m_connected[id]->axecount;
	}

	int joysticks::getCountButtons(const int &id)
	{
		return m_connected[id]->buttoncount;
	}
}
