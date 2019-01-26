#include "Joysticks.hpp"

#include <GLFW/glfw3.h>

namespace acid
{
	void CallbackJoystick(int32_t id, int32_t event)
	{
		auto &joystick = Joysticks::Get()->m_connected[id];

		if (event == GLFW_CONNECTED)
		{
			printf("Joystick connected: '%s' to %i\n", glfwGetJoystickName(id), id);
			joystick.m_connected = true;
			joystick.m_name = glfwGetJoystickName(id);
			Joysticks::Get()->m_onConnect(static_cast<JoystickPort>(id), true);
		}
		else if (event == GLFW_DISCONNECTED)
		{
			printf("Joystick disconnected from %i\n", id);
			joystick.m_connected = false;
			joystick.m_name = "";
			Joysticks::Get()->m_onConnect(static_cast<JoystickPort>(id), false);
		}
	}

	Joysticks::Joysticks() :
		m_connected(std::array<JoystickImpl, 16>()),
		m_onConnect(Delegate<void(JoystickPort, bool)>()),
		m_onButton(Delegate<void(JoystickPort, uint32_t, InputAction)>()),
		m_onAxis(Delegate<void(JoystickPort, uint32_t, float)>()),
		m_onHat(Delegate<void(JoystickPort, uint32_t, JoystickHat)>())
	{
		for (auto port : enum_iterator<JoystickPort>())
		{
			JoystickImpl joystick = {};
			joystick.m_connected = false;
			joystick.m_port = port;
			m_connected[static_cast<uint32_t>(port)] = joystick;

			if (glfwJoystickPresent(static_cast<uint32_t>(port)))
			{
				CallbackJoystick(static_cast<uint32_t>(port), GLFW_CONNECTED);
			}
		}

		glfwSetJoystickCallback(CallbackJoystick);
	}

	void Joysticks::Update()
	{
		for (auto &joystick : m_connected)
		{
			if (!joystick.m_connected)
			{
				continue;
			}

			int32_t axeCount = 0;
			auto axes = glfwGetJoystickAxes(static_cast<int32_t>(joystick.m_port), &axeCount);
			joystick.m_axes.resize(static_cast<uint32_t>(axeCount));

			for (uint32_t i = 0; i < axeCount; i++)
			{
				if (joystick.m_axes[i] != axes[i])
				{
					joystick.m_axes[i] = axes[i];
					m_onAxis(joystick.m_port, i, joystick.m_axes[i]);
				}
			}

			int32_t buttonCount = 0;
			auto buttons = glfwGetJoystickButtons(static_cast<int32_t>(joystick.m_port), &buttonCount);
			joystick.m_buttons.resize(static_cast<uint32_t>(buttonCount));

			for (uint32_t i = 0; i < buttonCount; i++)
			{
				if (buttons[i] != GLFW_RELEASE && joystick.m_buttons[i] != InputAction::Release)
				{
					joystick.m_buttons[i] = InputAction::Repeat;
				}
				else if (joystick.m_buttons[i] != static_cast<InputAction>(buttons[i]))
				{
					joystick.m_buttons[i] = static_cast<InputAction>(buttons[i]);
					m_onButton(joystick.m_port, i, joystick.m_buttons[i]);
				}
			}

			int32_t hatCount = 0;
			auto hats = glfwGetJoystickHats(static_cast<int32_t>(joystick.m_port), &hatCount);
			joystick.m_hats.resize(static_cast<uint32_t>(hatCount));

			for (uint32_t i = 0; i < hatCount; i++)
			{
				if (joystick.m_hats[i] != static_cast<JoystickHat>(hats[i]))
				{
					joystick.m_hats[i] = static_cast<JoystickHat>(hats[i]);
					m_onHat(joystick.m_port, i, joystick.m_hats[i]);
				}
			}
		}
	}

	float Joysticks::GetAxis(const JoystickPort &port, const uint32_t &axis) const
	{
		if (!IsConnected(port) || axis > GetAxisCount(port))
		{
			return false;
		}

		return m_connected.at(static_cast<uint32_t>(port)).m_axes[axis];
	}

	InputAction Joysticks::GetButton(const JoystickPort &port, const uint32_t &button) const
	{
		if (!IsConnected(port) || button > GetButtonCount(port))
		{
			return InputAction::Release;
		}

		return static_cast<InputAction>(m_connected.at(static_cast<uint32_t>(port)).m_buttons[button]);
	}

	JoystickHat Joysticks::GetHat(const JoystickPort &port, const uint32_t &hat) const
	{
		if (!IsConnected(port) || hat > GetHatCount(port))
		{
			return JOYSTICK_HAT_CENTERED;
		}

		return m_connected.at(static_cast<uint32_t>(port)).m_hats[hat];
	}
}
