#include "Joysticks.hpp"

namespace Flounder
{
	Joysticks::Joysticks() :
		IModule(),
		m_connected(new Joystick*[GLFW_JOYSTICK_LAST])
	{
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			m_connected[i] = new Joystick();
			m_connected[i]->id = i;
		}
	}

	Joysticks::~Joysticks()
	{
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			delete m_connected[i];
		}

		delete m_connected;
	}

	void Joysticks::Update()
	{
		// For each joystick check if connected and update.
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			Joystick *joy = m_connected[i];
			joy->id = i;

			if (glfwJoystickPresent(i))
			{
				if (!joy->connected)
				{
					printf("Joystick connected: '%s'.\n", glfwGetJoystickName(i));
				}

				joy->connected = true;
				joy->name = glfwGetJoystickName(i);
				joy->axes = glfwGetJoystickAxes(i, &joy->axeCount);
				joy->buttons = glfwGetJoystickButtons(i, &joy->buttonCount);
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
				joy->axeCount = 0;
				joy->buttonCount = 0;
			}
		}
	}

	float Joysticks::getAxis(const int &id, const int &axis) const
	{
		if (axis < 0 || axis > getCountAxes(id))
		{
			return false;
		}

		return m_connected[id]->axes[axis];
	}

	bool Joysticks::getButton(const int &id, const int &button) const
	{
		if (button < 0 || button > getCountButtons(id))
		{
			return false;
		}

		return m_connected[id]->buttons[button];
	}
}
