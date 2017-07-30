#include "aitasklevel.h"

const float aitasklevel::TIME_DELAY = 1.2f;

aitasklevel::aitasklevel(entity* object, const int &level, const bool &instant) :
	iaitask(object)
{
	m_level = level;
	m_instant = instant;
	m_timer = new timer(TIME_DELAY);
}

aitasklevel::~aitasklevel()
{
	delete m_timer;
}

void aitasklevel::start()
{
	m_timer->resetStartTime();
}

void aitasklevel::update()
{
	m_entity->getPosition()->set(0.5f, levelToPosition(m_level));

	// Walk anim.
	m_entity->setSelectedRow(0);
}

bool aitasklevel::isComplete()
{
	return m_instant || m_timer->isPassedTime();
}
