#pragma once

#include "../framework/framework.h"

#include "display.h"

namespace flounder
{
	/// <summary>
	/// A definition for a connected joystick.
	/// </summary>
	struct joystick
	{
		bool connected;
		int id;
		const char *name;
		const float *axes;
		const unsigned char *buttons;
		int axecount;
		int buttoncount;
	};

	/// <summary>
	/// A module used for the creation, updating and destruction of the joysticks.
	/// </summary>
	class joysticks :
		public imodule
	{
	private:
		joystick **m_connected;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline joysticks *get()
		{
			return static_cast<joysticks*>(framework::get()->getInstance("joysticks"));
		}

		/// <summary>
		/// Creates a new joysticks module.
		/// </summary>
		joysticks();

		/// <summary>
		/// Deconstructor for the joysticks module.
		/// </summary>
		~joysticks();

		void update() override;

		/// <summary>
		/// Determines if the GLFW joystick is connected
		/// </summary>
		/// <param name="id"> The joystick to check connection with. </param>
		/// <returns> If the joystick is connected. </returns>
		inline bool isConnected(const int &id) const { return m_connected[id]->connected; }

		/// <summary>
		/// Gets the name of the joystick.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <returns> The joysticks name. </returns>
		inline const char *getName(const int &id) const { return m_connected[id]->name; }

		/// <summary>
		/// Gets the value of a joysticks axis.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <param name="axis"> The axis of interest. </param>
		/// <returns> The value of the joystick's axis. </returns>
		float getAxis(const int &id, const int &axis) const;

		/// <summary>
		/// Gets the whether a button on a joystick is pressed.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <param name="button"> The button of interest. </param>
		/// <returns> Whether a button on a joystick is pressed. </returns>
		bool getButton(const int &id, const int &button) const;

		/// <summary>
		/// Gets the number of axes the joystick offers.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <returns> The number of axes the joystick offers. </returns>
		inline int getCountAxes(const int &id) const { return m_connected[id]->axecount; }

		/// <summary>
		/// Gets the number of buttons the joystick offers.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <returns> The number of buttons the joystick offers. </returns>
		inline int getCountButtons(const int &id) const { return m_connected[id]->buttoncount; }
	};
}
