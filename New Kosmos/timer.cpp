#include "timer.h"

timer::timer(float interval)
{
	//if (Framework.get().isInitialized()) {
	this->m_startTime = 0.0f; // TODO: Framework::get()->getTimeMs()
	//} else {
	//	this.startTime = 0.0f;
	//}

	this->m_interval = interval;
}

timer::~timer()
{
	delete &m_startTime;
	delete &m_interval;
}

bool timer::isPassedTime()
{
	return 0.0f - m_startTime > (m_interval * 1000.0f); // TODO: Framework::get()->getTimeMs()
}

void timer::resetStartTime()
{
	this->m_startTime = 0.0f; // TODO: Framework::get()->getTimeMs()
}
