#pragma once

#include <cmath>
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

		/// <summary>
		/// Gets the average UPS over a short interval.
		/// </summary>
		/// <returns> The UPS. </returns>
		const uint32_t &GetUps() const { return m_ups.m_value; }

		/// <summary>
		/// Gets the average FPS over a short interval.
		/// </summary>
		/// <returns> The FPS. </returns>
		const uint32_t &GetFps() const { return m_fps.m_value; }
	private:
		struct ChangePerSecond
		{
			uint32_t m_valueTemp, m_value;
			float m_valueTime;

			void Update(const float &time)
			{
				m_valueTemp++;

				if (std::floor(time) > std::floor(m_valueTime))
				{
					m_value = m_valueTemp;
					m_valueTemp = 0;
				}

				m_valueTime = time;
			}
		};

		Delta m_deltaUpdate;
		Delta m_deltaRender;
		Timer m_timerUpdate;
		Timer m_timerRender;

		ChangePerSecond m_ups, m_fps;
	};
}
