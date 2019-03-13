#include "Joysticks.hpp"

#include <GLFW/glfw3.h>

namespace acid
{
void CallbackJoystick(int32_t id, int32_t event)
{
	if(event == GLFW_CONNECTED)
		{
			printf("Joystick connected: '%s' to %i\n", glfwGetJoystickName(id), id);
			Joysticks::JoystickImpl joystick = {};
			joystick.m_name = glfwGetJoystickName(id);
			Joysticks::Get()->m_connected.emplace(id, joystick);
			Joysticks::Get()->m_onConnect(id, true);
		}
	else if(event == GLFW_DISCONNECTED)
		{
			printf("Joystick disconnected from %i\n", id);
			Joysticks::Get()->m_connected.erase(id);
			Joysticks::Get()->m_onConnect(id, false);
		}
}

Joysticks::Joysticks()
{
	glfwSetJoystickCallback(CallbackJoystick);

	for(uint32_t i = 0; i < GLFW_JOYSTICK_LAST; i++)
		{
			if(glfwJoystickPresent(i))
				{
					CallbackJoystick(i, GLFW_CONNECTED);
				}
		}
}

void Joysticks::Update()
{
	for(auto& [port, joystick] : m_connected)
		{
			int32_t axeCount = 0;
			auto axes = glfwGetJoystickAxes(port, &axeCount);
			joystick.m_axes.resize(static_cast<uint32_t>(axeCount));

			for(uint32_t i = 0; i < static_cast<uint32_t>(axeCount); i++)
				{
					if(joystick.m_axes[i] != axes[i])
						{
							joystick.m_axes[i] = axes[i];
							m_onAxis(port, i, joystick.m_axes[i]);
						}
				}

			int32_t buttonCount = 0;
			auto buttons = glfwGetJoystickButtons(port, &buttonCount);
			joystick.m_buttons.resize(static_cast<uint32_t>(buttonCount));

			for(uint32_t i = 0; i < static_cast<uint32_t>(buttonCount); i++)
				{
					if(buttons[i] != GLFW_RELEASE && joystick.m_buttons[i] != InputAction::Release)
						{
							joystick.m_buttons[i] = InputAction::Repeat;
						}
					else if(joystick.m_buttons[i] != static_cast<InputAction>(buttons[i]))
						{
							joystick.m_buttons[i] = static_cast<InputAction>(buttons[i]);
							m_onButton(port, i, joystick.m_buttons[i]);
						}
				}

			int32_t hatCount = 0;
			auto hats = glfwGetJoystickHats(port, &hatCount);
			joystick.m_hats.resize(static_cast<uint32_t>(hatCount));

			for(uint32_t i = 0; i < static_cast<uint32_t>(hatCount); i++)
				{
					if(joystick.m_hats[i] != MakeBitMask<JoystickHat>(hats[i]))
						{
							joystick.m_hats[i] = MakeBitMask<JoystickHat>(hats[i]);
							m_onHat(port, i, joystick.m_hats[i]);
						}
				}
		}
}

bool Joysticks::IsConnected(const uint32_t& port) const
{
	return GetJoystick(port).has_value();
}

std::string Joysticks::GetName(const uint32_t& port) const
{
	auto joystick = GetJoystick(port);
	return joystick ? joystick->m_name : "";
}

uint32_t Joysticks::GetAxisCount(const uint32_t& port) const
{
	auto joystick = GetJoystick(port);
	return joystick ? static_cast<uint32_t>(joystick->m_axes.size()) : 0;
}

uint32_t Joysticks::GetButtonCount(const uint32_t& port) const
{
	auto joystick = GetJoystick(port);
	return joystick ? static_cast<uint32_t>(joystick->m_buttons.size()) : 0;
}

uint32_t Joysticks::GetHatCount(const uint32_t& port) const
{
	auto joystick = GetJoystick(port);
	return joystick ? static_cast<uint32_t>(joystick->m_hats.size()) : 0;
}

float Joysticks::GetAxis(const uint32_t& port, const uint32_t& axis) const
{
	auto joystick = GetJoystick(port);

	if(!joystick || axis > joystick->m_axes.size())
		{
			return 0.0f;
		}

	return joystick->m_axes[axis];
}

InputAction Joysticks::GetButton(const uint32_t& port, const uint32_t& button) const
{
	auto joystick = GetJoystick(port);

	if(!joystick || button > joystick->m_buttons.size())
		{
			return InputAction::Release;
		}

	return joystick->m_buttons[button];
}

BitMask<JoystickHat> Joysticks::GetHat(const uint32_t& port, const uint32_t& hat) const
{
	auto joystick = GetJoystick(port);

	if(!joystick || hat > joystick->m_hats.size())
		{
			return Centered;
		}

	return joystick->m_hats[hat];
}

std::optional<Joysticks::JoystickImpl> Joysticks::GetJoystick(const uint32_t& port) const
{
	auto it = m_connected.find(port);

	if(it == m_connected.end())
		{
			return {};
		}

	return it->second;
}
}
