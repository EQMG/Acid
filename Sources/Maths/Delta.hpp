#pragma once

#include "Time.hpp"

namespace acid
{
/**
 * Class for handing and calculating deltas.
 */
class ACID_EXPORT Delta
{
public:
	/**
	 * Updates change and times.
	 */
	void Update();

	const Time &GetChange() const { return m_change; }

	const Time &GetTime() const { return m_time; }

private:
	Time m_currentFrameTime;
	Time m_lastFrameTime;
	Time m_change;
	Time m_time;
};
}
