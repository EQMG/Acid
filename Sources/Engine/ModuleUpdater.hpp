#pragma once

#include "Maths/Delta.hpp"
#include "Maths/Timer.hpp"
#include "ModuleHolder.hpp"

namespace acid
{
/**
 * @brief Class used to define how the engine will run updates and timings on modules.
 */
class ACID_EXPORT ModuleUpdater
{
public:
	ModuleUpdater();

	/**
	 * Updates all modules in order.
	 * @param moduleHolder The module holder to run updates on.
	 */
	void Update(ModuleHolder &moduleHolder);

	/**
	 * Gets the delta (seconds) between updates.
	 * @return The delta between updates.
	 */
	const Time &GetDelta() const { return m_deltaUpdate.GetChange(); }

	/**
	 * Gets the delta (seconds) between renders.
	 * @return The delta between renders.
	 */
	const Time &GetDeltaRender() const { return m_deltaRender.GetChange(); }

	/**
	 * Gets the average UPS over a short interval.
	 * @return The UPS.
	 */
	const uint32_t &GetUps() const { return m_ups.m_value; }

	/**
	 * Gets the average FPS over a short interval.
	 * @return The FPS.
	 */
	const uint32_t &GetFps() const { return m_fps.m_value; }

private:
	class ChangePerSecond
	{
	public:
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

		uint32_t m_valueTemp, m_value;
		float m_valueTime;
	};

	Delta m_deltaUpdate;
	Delta m_deltaRender;
	Timer m_timerUpdate;
	Timer m_timerRender;

	ChangePerSecond m_ups, m_fps;
};
}
