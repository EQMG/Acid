#pragma once

#include "../Engine/Engine.hpp"
#include "Display.hpp"

namespace Flounder
{
	/// <summary>
	/// A definition for a connected joystick.
	/// </summary>
	struct Joystick
	{
		bool connected;
		int id;
		const char *name;
		const float *axes;
		const unsigned char *buttons;
		int axeCount;
		int buttonCount;
	};

	/// <summary>
	/// A module used for the creation, updating and destruction of the joysticks.
	/// </summary>
	class Joysticks :
		public IModule
	{
	private:
		Joystick **m_connected;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Joysticks *Get()
		{
			return static_cast<Joysticks*>(Engine::Get()->GetModule("joysticks"));
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
		/// <param name="id"> The joystick to check connection with. </param>
		/// <returns> If the joystick is connected. </returns>
		bool IsConnected(const int &id) const { return m_connected[id]->connected; }

		/// <summary>
		/// Gets the name of the joystick.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <returns> The joysticks name. </returns>
		const char *GetName(const int &id) const { return m_connected[id]->name; }

		/// <summary>
		/// Gets the value of a joysticks axis.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <param name="axis"> The axis of interest. </param>
		/// <returns> The value of the joystick's axis. </returns>
		float GetAxis(const int &id, const int &axis) const;

		/// <summary>
		/// Gets the whether a button on a joystick is pressed.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <param name="button"> The button of interest. </param>
		/// <returns> Whether a button on a joystick is pressed. </returns>
		bool GetButton(const int &id, const int &button) const;

		/// <summary>
		/// Gets the number of axes the joystick offers.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <returns> The number of axes the joystick offers. </returns>
		int GetCountAxes(const int &id) const { return m_connected[id]->axeCount; }

		/// <summary>
		/// Gets the number of buttons the joystick offers.
		/// </summary>
		/// <param name="id"> The joystick of interest. </param>
		/// <returns> The number of buttons the joystick offers. </returns>
		int GetCountButtons(const int &id) const { return m_connected[id]->buttonCount; }
	};
}
