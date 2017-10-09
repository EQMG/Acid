#pragma once

#include <string>
#include "IModule.hpp"
#include "ModuleUpdate.hpp"

namespace Flounder
{
	/// <summary>
	/// A class used to define how the engine will run updates and timings.
	/// </summary>
	class IUpdater
	{
	public:
		/// <summary>
		/// Creates a new updater.
		/// </summary>
		IUpdater()
		{
		}

		/// <summary>
		/// Deconstructor for the updater.
		/// </summary>
		virtual ~IUpdater()
		{
		}

		virtual void Create() = 0;

		virtual void Update() = 0;

		/// <summary>
		/// Adds a module to the updater.
		/// </summary>
		/// <param name="typeUpdate"> The modules update type. </param>
		/// <param name="moduleName"> The modules name. </param>
		/// <param name="module"> The module object. </param>
		virtual void AddModule(ModuleUpdate typeUpdate, std::string moduleName, IModule *module) = 0;

		/// <summary>
		/// Gets a module from the updater.
		/// </summary>
		/// <param name="key"> The module name/key to get. </param>
		/// <returns> The module object. </returns>
		virtual IModule *GetModule(const std::string &name) = 0;

		/// <summary>
		/// Gets the added/removed time for the engine (seconds).
		/// </summary>
		/// <returns> The time offset. </returns>
		virtual float GetTimeOffset() = 0;

		/// <summary>
		/// Sets the time offset for the engine (seconds).
		/// </summary>
		/// <param name="timeOffset"> The new time offset. </param>
		virtual void SetTimeOffset(const float &timeOffset) = 0;

		/// <summary>
		/// Gets the delta (seconds) between updates.
		/// </summary>
		/// <returns> The delta between updates. </returns>
		virtual float GetDelta() = 0;

		/// <summary>
		/// Gets the delta (seconds) between renders.
		/// </summary>
		/// <returns> The delta between renders. </returns>
		virtual float GetDeltaRender() = 0;

		/// <summary>
		/// Gets the current time of the engine instance.
		/// </summary>
		/// <returns> The current engine time in seconds. </returns>
		virtual float GetTime() = 0;

		/// <summary>
		/// Gets the current time of the engine instance.
		/// </summary>
		/// <returns> The current engine time in milliseconds. </returns>
		virtual float GetTimeMs() = 0;
	};
}
