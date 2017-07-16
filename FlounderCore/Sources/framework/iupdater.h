#pragma once

#include <string>

#include "imodule.h"

namespace flounder {
	/// <summary>
	/// A class used to define how the framework will run updates and timings.
	/// </summary>
	class iupdater 
	{
	public:
		virtual void create() = 0;

		virtual void init() = 0;

		/// <summary>
		/// Function called by the framework to update the engine.
		/// </summary>
		virtual void update() = 0;

		/// <summary>
		/// Gets a module from the updater.
		/// </summary>
		/// <param name="key"> The module name/key to get. </param>
		/// <returns> The module object. </returns>
		virtual imodule *getInstance(const std::string &name) = 0;

		/// <summary>
		/// Gets the added/removed time for the framework (seconds).
		/// </summary>
		/// <returns> The time offset. </returns>
		virtual double getTimeOffset() = 0;

		/// <summary>
		/// Sets the time offset for the framework (seconds).
		/// </summary>
		/// <param name="timeOffset"> The new time offset. </param>
		virtual void setTimeOffset(const double &timeOffset) = 0;

		/// <summary>
		/// Gets the delta (seconds) between updates.
		/// </summary>
		/// <returns> The delta between updates. </returns>
		virtual double getDelta() = 0;

		/// <summary>
		/// Gets the delta (seconds) between renders.
		/// </summary>
		/// <returns> The delta between renders. </returns>
		virtual double getDeltaRender() = 0;

		/// <summary>
		/// Sets the FPS limit for the updater, called by the framework.
		/// </summary>
		/// <param name="fpsLimit"> The new fps limit. </param>
		virtual void setFpsLimit(const double &fpsLimit) = 0;

		/// <summary>
		/// Gets the current time of the framework instance.
		/// </summary>
		/// <returns> The current framework time in seconds. </returns>
		virtual double getTimeSec() = 0;

		/// <summary>
		/// Gets the current time of the framework instance.
		/// </summary>
		/// <returns> The current framework time in milliseconds. </returns>
		virtual double getTimeMs() = 0;
	};
}