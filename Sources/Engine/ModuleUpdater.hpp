#pragma once

#include "Maths/Delta.hpp"
#include "Maths/Timer.hpp"
#include "ModuleManager.hpp"

namespace acid
{
	/// <summary>
	/// A class used to define how the engine will run updates and timings on modules.
	/// </summary>
	class ACID_EXPORT ModuleUpdater
	{
	public:
		ModuleUpdater();

		/// <summary>
		/// Updates all modules in order.
		/// </summary>
		void Update(ModuleManager &moduleManager);

		/// <summary>
		/// Gets the delta (seconds) between updates.
		/// </summary>
		/// <returns> The delta between updates. </returns>
		const Time &GetDelta() const { return m_deltaUpdate.GetChange(); }

		/// <summary>
		/// Gets the delta (seconds) between renders.
		/// </summary>
		/// <returns> The delta between renders. </returns>
		const Time &GetDeltaRender() const { return m_deltaRender.GetChange(); }
	private:
		Delta m_deltaUpdate;
		Delta m_deltaRender;
		Timer m_timerUpdate;
		Timer m_timerRender;
	};
}