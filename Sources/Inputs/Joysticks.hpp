#pragma once

#include "Engine/Engine.hpp"
#include "Display/Display.hpp"

namespace fl
{
	/// <summary>
	/// A definition for a managed joystick.
	/// </summary>
	struct Joystick
	{
		JoystickPort m_port;
		bool m_connected;
		std::string m_name;
		std::vector<bool> m_buttons;
		std::vector<float> m_axes;
	};

	/// <summary>
	/// A module used for the creation, updating and destruction of the joysticks.
	/// </summary>
	class FL_EXPORT Joysticks :
		public IModule
	{
	private:
		std::array<Joystick, JOYSTICK_END_RANGE> m_connected;

		friend void CallbackJoystickConnect(JoystickPort port, std::string name, uint32_t buttonCount, uint32_t axesCount, bool connected);

		friend void CallbackJoystickButton(JoystickPort port, uint32_t button, bool isDown);

		friend void CallbackJoystickAxis(JoystickPort port, uint32_t axis, float amount);

	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Joysticks *Get()
		{
			return Engine::Get()->GetModule<Joysticks>();
		}

		/// <summary>
		/// Creates a new joysticks module.
		/// </summary>
		Joysticks();

		/// <summary>
		/// Deconstructor for the joysticks module.
		/// </summary>
		~Joysticks();

		void Update() override;

		std::string GetName() const override { return "Joysticks"; };

		/// <summary>
		/// Determines if the joystick is connected
		/// </summary>
		/// <param name="port"> The joystick to check connection with. </param>
		/// <returns> If the joystick is connected. </returns>
		bool IsConnected(const JoystickPort &port) const { return m_connected.at(port).m_connected; }

		/// <summary>
		/// Gets the name of the joystick.
		/// </summary>
		/// <param name="port"> The joystick to get the name of. </param>
		/// <returns> The joysticks name. </returns>
		std::string GetName(const JoystickPort &port) const { return m_connected.at(port).m_name; }

		/// <summary>
		/// Gets the whether a button on a joystick is pressed.
		/// </summary>
		/// <param name="port"> The joystick to get the button from. </param>
		/// <param name="button"> The button of interest. </param>
		/// <returns> Whether a button on a joystick is pressed. </returns>
		bool GetButton(const JoystickPort &port, const uint32_t &button) const;

		/// <summary>
		/// Gets the value of a joysticks axis.
		/// </summary>
		/// <param name="port"> The joystick to get the axis from. </param>
		/// <param name="axis"> The axis of interest. </param>
		/// <returns> The value of the joystick's axis. </returns>
		float GetAxis(const JoystickPort &port, const uint32_t &axis) const;

		/// <summary>
		/// Gets the number of buttons the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the button count from. </param>
		/// <returns> The number of buttons the joystick offers. </returns>
		uint32_t GetCountButtons(const JoystickPort &port) const { return m_connected.at(port).m_buttons.size(); }

		/// <summary>
		/// Gets the number of axes the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the axis count from. </param>
		/// <returns> The number of axes the joystick offers. </returns>
		uint32_t GetCountAxes(const JoystickPort &port) const { return m_connected.at(port).m_axes.size(); }
	};
}
