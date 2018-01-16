#include "Joysticks.hpp"

namespace Flounder
{
	Joysticks::Joysticks() :
		IModule(),
		m_connected(new Joystick[GLFW_JOYSTICK_LAST])
	{
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			m_connected[i] = Joystick();
			m_connected[i].id = i;
		}
	}

	Joysticks::~Joysticks()
	{
		delete[] m_connected;
	}

	void Joysticks::Update()
	{
		// For each joystick check if connected and update.
		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			Joystick joy = m_connected[i];
			joy.id = i;

			if (glfwJoystickPresent(i))
			{
				if (!joy.connected)
				{
					printf("Joystick connected: '%s'.\n", glfwGetJoystickName(i));
				}

				joy.connected = true;
				joy.name = glfwGetJoystickName(i);
				joy.axes = glfwGetJoystickAxes(i, &joy.axeCount);
				joy.buttons = glfwGetJoystickButtons(i, &joy.buttonCount);
			}
			else
			{
				if (joy.connected)
				{
					printf("Joystick disconnected!\n");
				}

				joy.connected = false;
				joy.name = "";
				joy.axes = nullptr;
				joy.buttons = nullptr;
				joy.axeCount = 0;
				joy.buttonCount = 0;
			}
		}
	}

	float Joysticks::GetAxis(const int &id, const int &axis) const
	{
		if (axis < 0 || axis > GetCountAxes(id))
		{
			return false;
		}

		return m_connected[id].axes[axis];
	}

	bool Joysticks::GetButton(const int &id, const int &button) const
	{
		if (button < 0 || button > GetCountButtons(id))
		{
			return false;
		}

		return m_connected[id].buttons[button];
	}
}
