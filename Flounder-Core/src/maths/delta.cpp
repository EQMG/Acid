#include "delta.hpp"

namespace flounder
{
	delta::delta() :
		m_currentFrameTime(0.0f),
		m_lastFrameTime(0.0f),
		m_change(0.0f),
		m_time(0.0f)
	{
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
}
