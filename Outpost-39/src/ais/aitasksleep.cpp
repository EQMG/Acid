#include "aitasksleep.h"

aitasksleep::aitasksleep(entity* object) :
	iaitask(object)
{
	m_sleeping = true;
}

aitasksleep::~aitasksleep()
{
}

void aitasksleep::start()
{
}

void aitasksleep::update()
{
	float time = cos(framework::get()->getTimeSec() / instance::DAY_LENGTH);

	if (time < 0.1f)
	{
		m_entity->setSelectedRow(1);
		m_sleeping = true;
	}
	else
	{
		m_entity->setSelectedRow(0);
		m_sleeping = false;
	}
}

bool aitasksleep::isComplete()
{
	return !m_sleeping;
}
