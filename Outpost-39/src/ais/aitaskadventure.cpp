#include "aitaskadventure.h"

const float aitaskadventure::LENGTH = 20.0f;

aitaskadventure::aitaskadventure(entity* object) :
	iaitask(object)
{
	m_timer = new timer(LENGTH);
	complete = false;
}

aitaskadventure::~aitaskadventure()
{
	delete m_timer;
}

void aitaskadventure::update()
{
	if (m_timer->isPassedTime())
	{
		m_entity->setAlpha(1.0f);
		m_entity->getPosition()->set(0.5f, levelToPosition(0));
		complete = true;
	}
	else
	{
		m_entity->setAlpha(0.0f);
	}
}

bool aitaskadventure::isComplete()
{
	return complete;
}
