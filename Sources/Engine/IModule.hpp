#pragma once

#include "Exports.hpp"

namespace acid
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
	class ACID_EXPORT IModule
	{
	public:
		/// <summary>
		/// The update function for the module.
		/// </summary>
		virtual void Update() = 0;
	};
}
