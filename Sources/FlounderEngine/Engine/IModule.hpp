#pragma once

#include "Exports.hpp"

namespace fl
{
	/// <summary>
	/// Represents when a module will update in the game loop.
	/// </summary>
	enum ModuleUpdate
	{
		UpdateAlways = 0,
		UpdatePre = 1,
		UpdateNormal = 2,
		UpdatePost = 3,
		UpdateRender = 4
	};

	/// <summary>
	/// A interface used for defining engine modules.
	/// </summary>
	class FL_EXPORT IModule
	{
	public:
		/// <summary>
		/// Creates a new module.
		/// </summary>
		IModule()
		{
		}

		/// <summary>
		/// Deconstructor for the module.
		/// </summary>
		virtual ~IModule()
		{
		}

		/// <summary>
		/// The update function for the module.
		/// </summary>
		virtual void Update() = 0;
	};
}
