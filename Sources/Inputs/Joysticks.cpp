#include "Joysticks.hpp"

namespace fl
{
	void CallbackJoystickConnect(WsiShell shell, WsiJoystick port, const char *name, uint32_t buttonCount, uint32_t axesCount, VkBool32 connected)
	{
#if FL_VERBOSE
		fprintf(stdout, "Joystick '%s' %s port %i\n", name, connected ? "connected to" : "disconnected from ", port);
#endif
		Joystick joystick = Joysticks::Get()->m_connected[port];
		bool changed = joystick.m_connected != connected;
		joystick.m_connected = connected;
		joystick.m_name = name;
		joystick.m_buttons.resize(buttonCount);
		joystick.m_axes.resize(axesCount);

		if (changed)
		{
			for (int i = 0; i < buttonCount; i++)
			{
				joystick.m_buttons[i] = WSI_ACTION_RELEASE;
			}

			for (int i = 0; i < axesCount; i++)
			{
				joystick.m_axes[i] = 0.0f;
			}
		}
	}

	void CallbackJoystickButton(WsiShell shell, WsiJoystick port, uint32_t button, WsiAction action)
	{
		Joystick joystick = Joysticks::Get()->m_connected[port];
		joystick.m_buttons[button] = action;
	}

	void CallbackJoystickAxis(WsiShell shell, WsiJoystick port, uint32_t axis, float amount)
	{
		Joystick joystick = Joysticks::Get()->m_connected[port];
		joystick.m_axes[axis] = amount;
	}

	Joysticks::Joysticks() :
		IModule(),
		m_connected(std::array<Joystick, WSI_JOYSTICK_END_RANGE>())
	{
		for (int i = 0; i < WSI_JOYSTICK_END_RANGE; i++)
		{
			Joystick joystick = Joystick();
			joystick.m_port = static_cast<WsiJoystick>(i);
			joystick.m_connected = false;
			m_connected[i] = joystick;
		}

		// Sets the joystick callbacks.
		WsiShellCallbacks *callbacks;
		wsiGetShellCallbacks(Display::Get()->GetWsiShell(), &callbacks);
		callbacks->pfnJoystickConnect = CallbackJoystickConnect;
		callbacks->pfnJoystickButton = CallbackJoystickButton;
		callbacks->pfnJoystickAxis = CallbackJoystickAxis;
	}

	Joysticks::~Joysticks()
	{
	}

	void Joysticks::Update()
	{
	}

	bool Joysticks::GetButton(const WsiJoystick &port, const uint32_t &button) const
	{
		if (button > GetCountButtons(port))
		{
			return false;
		}

		return m_connected.at(port).m_buttons[button];
	}

	float Joysticks::GetAxis(const WsiJoystick &port, const uint32_t &axis) const
	{
		if (axis > GetCountAxes(port))
		{
			return false;
		}

		return m_connected.at(port).m_axes[axis];
	}
}
