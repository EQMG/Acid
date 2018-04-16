#pragma once

#include "Maths/Colour.hpp"

namespace Flounder
{
	/// <summary>
	/// A interface used to manage a main UI system.
	/// </summary>
	class F_EXPORT IManagerUis
	{
	public:
		/// <summary>
		/// Creates a new ui manager.
		/// </summary>
		IManagerUis()
		{
		}

		/// <summary>
		/// Deconstructor for the ui manager.
		/// </summary>
		virtual ~IManagerUis()
		{
		}

		/// <summary>
		/// Run when updating the ui manager.
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// Gets if the main menu is open.
		/// </summary>
		/// <returns> If the main menu is open. </returns>
		virtual bool IsGamePaused() = 0;

		/// <summary>
		/// Gets the main menu's blur factor.
		/// </summary>
		/// <returns> The main menu's blur factor. </returns>
		virtual float GetBlurFactor() = 0;

		/// <summary>
		/// The primary colour to be used in UI elements.
		/// </summary>
		/// <returns> The primary colour. </returns>
		virtual Colour *GetPrimaryColour() = 0;
	};
}
