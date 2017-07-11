#pragma once

#include "../framework/framework.h"

#include "display.h"

namespace flounder {
	/// <summary>
	/// A module used for the creation, updating and destruction of the joysticks.
	/// </summary>
	class joysticks : public imodule
	{
	private:
		/// <summary>
		/// A definition for a connected joystick.
		/// </summary>
		struct joystick 
		{
			bool connected = false;
			int id;
			const char *name;
			const float *axes;
			int axecount;
			const unsigned char *buttons;
			int buttoncount;
		};

		joystick **m_connected;
	public:
		static joysticks* get() 
		{
			return (joysticks*) framework::get()->getInstance("joysticks");
		}

		/// <summary>
		/// Creates a new GLFW joysticks manager.
		/// </summary>
		joysticks();

		/// <summary>
		/// Deconstructor for the joysticks.
		/// </summary>
		~joysticks();

		void init();

		void update();

		/// <summary>
		/// Determines if the GLFW joystick is connected
		/// </summary>
		/// <param name="id"> The joystick to check connection with.
		/// </param>
		/// <returns> If the joystick is connected. </returns>
		bool isConnected(const int id);

		/// <summary>
		/// Gets the name of the joystick.
		/// </summary>
		/// <param name="id"> The joystick of interest.
		/// </param>
		/// <returns> The joysticks name. </returns>
		const char *getName(const int id);

		/// <summary>
		/// Gets the value of a joystick's axis.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <param name="axis"> The axis of interest.
		/// </param>
		/// <returns> The value of the joystick's axis. </returns>
		float getAxis(const int id, const int axis);

		/// <summary>
		/// Gets the whether a button on a joystick is pressed.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <param name="button"> The button of interest.
		/// </param>
		/// <returns> Whether a button on a joystick is pressed. </returns>
		bool getButton(const int id, const int button);

		/// <summary>
		/// Gets the number of axes the joystick offers.
		/// </summary>
		/// <param name="id"> The joystick of interest.
		/// </param>
		/// <returns> The number of axes the joystick offers. </returns>
		int getCountAxes(const int id);

		/// <summary>
		/// Gets the number of buttons the joystick offers.
		/// </summary>
		/// <param name="id"> The joystick of interest.
		/// </param>
		/// <returns> The number of buttons the joystick offers. </returns>
		int getCountButtons(const int id);
	};
}
