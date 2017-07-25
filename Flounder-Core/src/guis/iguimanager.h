#pragma once

#include "../maths/colour.h"

namespace flounder
{
	/// <summary>
	/// A interface used to manage a main GUI system.
	/// </summary>
	class iguimanager
	{
	public:
		/// <summary>
		/// Creates a new gui master.
		/// </summary>
		iguimanager()
		{
		}

		/// <summary>
		/// Deconstructor for the gui master.
		/// </summary>
		virtual ~iguimanager()
		{
		}

		/// <summary>
		/// Run when updating the GUI master.
		/// </summary>
		virtual void update() = 0;

		/// <summary>
		/// Gets if the main menu is open.
		/// </summary>
		/// <returns> If the main menu is open. </returns>
		virtual bool isGamePaused() = 0;

		/// <summary>
		/// Gets the main menu's blur factor.
		/// </summary>
		/// <returns> The main menu's blur factor. </returns>
		virtual float getBlurFactor() = 0;

		/// <summary>
		/// The primary colour to be used in UI elements.
		/// </summary>
		/// <returns> The primary colour. </returns>
		virtual colour *getPrimaryColour() = 0;
	};
}
