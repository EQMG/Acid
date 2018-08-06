#pragma once

#include "Exports.hpp"
#include "ModuleRegister.hpp"

namespace acid
{
	/// <summary>
	/// A class used to define how the engine will run updates and timings.
	/// </summary>
	class ACID_EXPORT IUpdater
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

		/// <summary>
		/// Updates all modules in order.
		/// </summary>
		/// <returns> The module register. </returns>
		virtual void Update(const ModuleRegister &moduleRegister) = 0;

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
	};
}
