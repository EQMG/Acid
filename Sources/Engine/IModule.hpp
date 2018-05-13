#pragma once

#include "Exports.hpp"

namespace fl
{
	/// <summary>
	/// Represents when a module will update in the game loop.
	/// </summary>
	enum ModuleUpdate
	{
		UPDATE_ALWAYS = 0,
		UPDATE_PRE = 1,
		UPDATE_NORMAL = 2,
		UPDATE_POST = 3,
		UPDATE_RENDER = 4
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
