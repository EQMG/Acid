#include "Joysticks.hpp"

namespace fl
{
	Joysticks::Joysticks() :
		IModule(),
		m_connected(new std::vector<Joystick *>())
	{
		for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; i++)
		{
			Joystick *joystick = new Joystick();
			joystick->m_id = i;
			joystick->m_connected = false;
			m_connected->push_back(joystick);
		}
	}

	Joysticks::~Joysticks()
	{
		for (auto joystick : *m_connected)
		{
			delete joystick;
		}

		delete m_connected;
	}

	void Joysticks::Update()
	{
		// For each joystick check if connected and update.
		for (auto joystick : *m_connected)
		{
			if (glfwJoystickPresent(joystick->m_id))
			{
				if (!joystick->m_connected)
				{
					printf("Joystick connected: '%s' to %i\n", glfwGetJoystickName(joystick->m_id), joystick->m_id);
					joystick->m_connected = true;
					joystick->m_name = glfwGetJoystickName(joystick->m_id);
				}

				joystick->m_axes = glfwGetJoystickAxes(joystick->m_id, &joystick->m_axeCount);
				joystick->m_buttons = glfwGetJoystickButtons(joystick->m_id, &joystick->m_buttonCount);
			}
			else
			{
				if (joystick->m_connected)
				{
					printf("Joystick disconnected from %i\n", joystick->m_id);
					joystick->m_connected = false;
					joystick->m_name = "";
					joystick->m_axes = nullptr;
					joystick->m_buttons = nullptr;
					joystick->m_axeCount = 0;
					joystick->m_buttonCount = 0;
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

		return m_connected->at(id)->m_axes[axis];
	}

	bool Joysticks::GetButton(const unsigned int &id, const int &button) const
	{
		if (button < 0 || button > GetCountButtons(id))
		{
			return false;
		}

		return m_connected->at(id)->m_buttons[button];
	}
}
