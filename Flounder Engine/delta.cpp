#include "delta.h"

namespace flounder {
	delta::delta()
	{
		this->m_currentFrameTime = 0.0;
		this->m_lastFrameTime = 0.0;

		this->m_change = 0.0;
		this->m_time = 0.0;
	}

	delta::~delta()
	{
	//	delete &m_currentFrameTime;
	//	delete &m_lastFrameTime;

	//	delete &m_change;
	//	delete &m_time;
	}

	void delta::update()
	{
		m_currentFrameTime = 0.0; // TODO: Framework->get().getTimeMs() / 1000.0;
		m_change = m_currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = m_currentFrameTime;
		m_time += m_change;
	}
}
