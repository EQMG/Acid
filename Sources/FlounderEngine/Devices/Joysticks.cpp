#include "Joysticks.hpp"

namespace Flounder
{
	Joysticks::Joysticks() :
		IModule(),
		m_connected(std::vector<Joystick *>())
	{
		for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; i++)
		{
			Joystick *joystick = new Joystick();
			joystick->id = i;
			joystick->connected = false;
			m_connected.push_back(joystick);
		}
	}

	Joysticks::~Joysticks()
	{
	}

	void Joysticks::Update()
	{
		// For each joystick check if connected and update.
		for (auto joystick : m_connected)
		{
			if (glfwJoystickPresent(joystick->id))
			{
				if (!joystick->connected)
				{
					printf("Joystick connected: '%s' to %i\n", glfwGetJoystickName(joystick->id), joystick->id);
					joystick->connected = true;
					joystick->name = glfwGetJoystickName(joystick->id);
				}

				joystick->axes = glfwGetJoystickAxes(joystick->id, &joystick->axeCount);
				joystick->buttons = glfwGetJoystickButtons(joystick->id, &joystick->buttonCount);
			}
			else
			{
				if (joystick->connected)
				{
					printf("Joystick disconnected from %i\n", joystick->id);
					joystick->connected = false;
					joystick->name = "";
					joystick->axes = nullptr;
					joystick->buttons = nullptr;
					joystick->axeCount = 0;
					joystick->buttonCount = 0;
				}
			}
		}
	}

	float Joysticks::GetAxis(const unsigned int &id, const int &axis) const
	{
		if (axis < 0 || axis > GetCountAxes(id))
		{
			return false;
		}

		return m_connected[id]->axes[axis];
	}

	bool Joysticks::GetButton(const unsigned int &id, const int &button) const
	{
		if (button < 0 || button > GetCountButtons(id))
		{
			return false;
		}

		return m_connected[id]->buttons[button];
	}
}
