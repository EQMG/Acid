#include "Joysticks.hpp"

#include <GLFW/glfw3.h>
#include "Renderer/Renderer.hpp"

namespace acid
{
	Joysticks::Joysticks() :
		m_connected(std::array<JoystickImpl, JOYSTICK_END_RANGE>())
	{
		for (int32_t i = 0; i < JOYSTICK_END_RANGE; i++)
		{
			JoystickImpl joystick = {};
			joystick.m_connected = false;
			joystick.m_port = static_cast<JoystickPort>(i);
			m_connected[i] = joystick;
		}
	}

	void Joysticks::Update()
	{
		for (auto &joystick : m_connected)
		{
			if (glfwJoystickPresent(joystick.m_port))
			{
				if (!joystick.m_connected)
				{
					printf("Joystick connected: '%s' to %i\n", glfwGetJoystickName(joystick.m_port), joystick.m_port);
					joystick.m_connected = true;
					joystick.m_name = glfwGetJoystickName(joystick.m_port);
				}

				int32_t axeCount = 0;
				int32_t buttonCount = 0;
				int32_t hatCount = 0;
				joystick.m_axes = glfwGetJoystickAxes(joystick.m_port, &axeCount);
				joystick.m_buttons = glfwGetJoystickButtons(joystick.m_port, &buttonCount);
				joystick.m_hats = glfwGetJoystickHats(joystick.m_port, &hatCount);
				joystick.m_axeCount = static_cast<uint32_t>(axeCount);
				joystick.m_buttonCount = static_cast<uint32_t>(buttonCount);
				joystick.m_hatCount = static_cast<uint32_t>(hatCount);
			}
			else
			{
				if (joystick.m_connected)
				{
					printf("Joystick disconnected from %i\n", joystick.m_port);
					joystick.m_connected = false;
					joystick.m_name = "";
					joystick.m_axes = nullptr;
					joystick.m_buttons = nullptr;
					joystick.m_axeCount = 0;
					joystick.m_buttonCount = 0;
				}
			}
		}
	}

	bool Joysticks::GetButton(const JoystickPort &port, const uint32_t &button) const
	{
		if (button > GetButtonCount(port))
		{
			return false;
		}

		return m_connected.at(port).m_buttons[button];
	}

	float Joysticks::GetAxis(const JoystickPort &port, const uint32_t &axis) const
	{
		if (axis > GetAxisCount(port))
		{
			return false;
		}

		return m_connected.at(port).m_axes[axis];
	}

	JoystickHatFlags Joysticks::GetHat(const JoystickPort &port, const uint32_t &hat) const
	{
		if (hat > GetHatCount(port))
		{
			return JOYSTICK_HAT_CENTERED;
		}

		return m_connected.at(port).m_hats[hat];
	}
}
