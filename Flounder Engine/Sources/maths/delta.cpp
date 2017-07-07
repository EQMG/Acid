#include "delta.h"

namespace flounder {
	delta::delta()
	{
		m_currentFrameTime = 0.0;
		m_lastFrameTime = 0.0;

		m_change = 0.0;
		m_time = 0.0;
	}

	delta::~delta()
	{
	}

	void delta::update()
	{
		m_currentFrameTime = framework::get()->getTimeSec();
		m_change = m_currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = m_currentFrameTime;
		m_time += m_change;
	}

	double delta::getChange()
	{
		return m_change;
	}

	double delta::getTime()
	{
		return m_time;
	}
}
