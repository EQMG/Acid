#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Exports.hpp"

namespace acid
{
	/// <summary>
	/// Represents when a module will call <seealso cref="Module#Update()"/> in the update loop.
	/// </summary>
	enum ModuleUpdate
	{
		MODULE_UPDATE_ALWAYS = 0,
		MODULE_UPDATE_PRE = 1,
		MODULE_UPDATE_NORMAL = 2,
		MODULE_UPDATE_POST = 3,
		MODULE_UPDATE_RENDER = 4
	};

	/// <summary>
	/// A interface used for defining engine modules.
	/// </summary>
	class ACID_EXPORT Module :
		public NonCopyable
	{
	public:
		/// <summary>
		/// The update function for the module.
		/// </summary>
		virtual void Update() = 0;
	};
}
