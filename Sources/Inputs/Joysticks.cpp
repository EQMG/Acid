#include "Joysticks.hpp"

namespace acid
{
	void CallbackJoystickConnect(JoystickPort port, std::string name, uint32_t buttonCount, uint32_t axesCount, bool connected)
	{
#if ACID_VERBOSE
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
				joystick.m_buttons[i] = false;
			}

			for (int i = 0; i < axesCount; i++)
			{
				joystick.m_axes[i] = 0.0f;
			}
		}
	}

	void CallbackJoystickButton(JoystickPort port, uint32_t button, bool isDown)
	{
		Joystick &joystick = Joysticks::Get()->m_connected[port];
		joystick.m_buttons[button] = isDown;
	}

	void CallbackJoystickAxis(JoystickPort port, uint32_t axis, float amount)
	{
		Joystick &joystick = Joysticks::Get()->m_connected[port];
		joystick.m_axes[axis] = amount;
	}

	Joysticks::Joysticks() :
		IModule(),
		m_connected(std::array<Joystick, JOYSTICK_END_RANGE>())
	{
		auto shell = Display::Get()->GetShell();

		for (int i = 0; i < JOYSTICK_END_RANGE; i++)
		{
			Joystick joystick = Joystick();
			joystick.m_port = static_cast<JoystickPort>(i);
			joystick.m_connected = false;
			m_connected[i] = joystick;
		}

		// Sets the joystick callbacks.
		shell->SetCallbackJoystickConnect(CallbackJoystickConnect);
		shell->SetCallbackJoystickButton(CallbackJoystickButton);
		shell->SetCallbackJoystickAxis(CallbackJoystickAxis);
	}

	Joysticks::~Joysticks()
	{
	}

	void Joysticks::Update()
	{
	}

	bool Joysticks::GetButton(const JoystickPort &port, const uint32_t &button) const
	{
		if (button > GetCountButtons(port))
		{
			return false;
		}

		return m_connected.at(port).m_buttons[button];
	}

	float Joysticks::GetAxis(const JoystickPort &port, const uint32_t &axis) const
	{
		if (axis > GetCountAxes(port))
		{
			return false;
		}

		return m_connected.at(port).m_axes[axis];
	}
}
