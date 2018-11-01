#pragma once

#include "Exports.hpp"
#include "Maths/Delta.hpp"
#include "Maths/Timer.hpp"
#include "ModuleRegister.hpp"

namespace acid
{
	/// <summary>
	/// A class used to define how the engine will run updates and timings on modules.
	/// </summary>
	class ACID_EXPORT ModuleUpdater
	{
	private:
		Delta m_deltaUpdate;
		Delta m_deltaRender;
		Timer m_timerUpdate;
		Timer m_timerRender;
	public:
		ModuleUpdater();

		/// <summary>
		/// Updates all modules in order.
		/// </summary>
		/// <returns> The module register. </returns>
		void Update(const ModuleRegister &moduleRegister);

		/// <summary>
		/// Gets the delta (seconds) between updates.
		/// </summary>
		/// <returns> The delta between updates. </returns>
		Time GetDelta() const { return m_deltaUpdate.GetChange(); }

		/// <summary>
		/// Gets the delta (seconds) between renders.
		/// </summary>
		/// <returns> The delta between renders. </returns>
		Time GetDeltaRender() const { return m_deltaRender.GetChange(); }
	};
}