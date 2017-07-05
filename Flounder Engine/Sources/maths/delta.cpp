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
	//	delete &m_currentFrameTime;
	//	delete &m_lastFrameTime;

	//	delete &m_change;
	//	delete &m_time;
	}

	void delta::update(float timeMs)
	{
		m_currentFrameTime = timeMs / 1000.0;
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
