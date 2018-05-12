#pragma once

#include "Engine/Engine.hpp"
#include "Display/Display.hpp"

namespace fl
{
	enum JoystickPort
	{
		JOYSTICK_1 = 0,
		JOYSTICK_2 = 1,
		JOYSTICK_3 = 2,
		JOYSTICK_4 = 3,
		JOYSTICK_5 = 4,
		JOYSTICK_6 = 5,
		JOYSTICK_7 = 6,
		JOYSTICK_8 = 7,
		JOYSTICK_9 = 8,
		JOYSTICK_10 = 9,
		JOYSTICK_11 = 10,
		JOYSTICK_12 = 11,
		JOYSTICK_13 = 12,
		JOYSTICK_14 = 13,
		JOYSTICK_15 = 14,
		JOYSTICK_16 = 15,
		JOYSTICK_LAST = JOYSTICK_16
	};

	/// <summary>
	/// A definition for a connected joystick.
	/// </summary>
	struct FL_HIDDEN Joystick
	{
	public:
		bool m_connected;
		int m_id;
		const char *m_name;
		const float *m_axes;
		const unsigned char *m_buttons;
		int m_axeCount;
		int m_buttonCount;
	};

	/// <summary>
	/// A module used for the creation, updating and destruction of the joysticks.
	/// </summary>
	class FL_EXPORT Joysticks :
		public IModule
	{
	private:
		std::vector<Joystick *> *m_connected;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Joysticks *Get()
		{
			return reinterpret_cast<Joysticks *>(Engine::Get()->GetModule("joysticks"));
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

		/// <summary>
		/// Determines if the GLFW joystick is connected
		/// </summary>
		/// <param name="port"> The joystick to check connection with. </param>
		/// <returns> If the joystick is connected. </returns>
		bool IsConnected(const JoystickPort &port) const { return m_connected->at(port)->m_connected; }

		/// <summary>
		/// Gets the name of the joystick.
		/// </summary>
		/// <param name="port"> The joystick to get the name of. </param>
		/// <returns> The joysticks name. </returns>
		const char *GetName(const JoystickPort &port) const { return m_connected->at(port)->m_name; }

		/// <summary>
		/// Gets the value of a joysticks axis.
		/// </summary>
		/// <param name="port"> The joystick to get the axis from. </param>
		/// <param name="axis"> The axis of interest. </param>
		/// <returns> The value of the joystick's axis. </returns>
		float GetAxis(const JoystickPort &port, const int &axis) const;

		/// <summary>
		/// Gets the whether a button on a joystick is pressed.
		/// </summary>
		/// <param name="port"> The joystick to get the button from. </param>
		/// <param name="button"> The button of interest. </param>
		/// <returns> Whether a button on a joystick is pressed. </returns>
		bool GetButton(const JoystickPort &port, const int &button) const;

		/// <summary>
		/// Gets the number of axes the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the axis count from. </param>
		/// <returns> The number of axes the joystick offers. </returns>
		int GetCountAxes(const JoystickPort &port) const { return m_connected->at(port)->m_axeCount; }

		/// <summary>
		/// Gets the number of buttons the joystick offers.
		/// </summary>
		/// <param name="port"> The joystick to the the button count from. </param>
		/// <returns> The number of buttons the joystick offers. </returns>
		int GetCountButtons(const JoystickPort &port) const { return m_connected->at(port)->m_buttonCount; }
	};
}
