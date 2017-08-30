#include "joysticks.hpp"

namespace flounder
{
	joysticks::joysticks() :
		imodule(),
		m_connected(new joystick*[GLFW_JOYSTICK_LAST])
	{
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			m_connected[i] = new joystick();
			m_connected[i]->id = i;
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
					printf("Joystick connected: '%s'.\n", glfwGetJoystickName(i));
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
					printf("Joystick disconnected!\n");
				}

				joy->connected = false;
				joy->name = "";
				joy->axes = nullptr;
				joy->buttons = nullptr;
				joy->axecount = 0;
				joy->buttoncount = 0;
			}
		}
	}

	float joysticks::getAxis(const int &id, const int &axis) const
	{
		if (axis < 0 || axis > getCountAxes(id))
		{
			return false;
		}

		return m_connected[id]->axes[axis];
	}

	bool joysticks::getButton(const int &id, const int &button) const
	{
		if (button < 0 || button > getCountButtons(id))
		{
			return false;
		}

		return m_connected[id]->buttons[button];
	}
}
