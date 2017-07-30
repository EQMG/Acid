#include "aitaskfade.h"

const float aitaskfade::TIME_FADE = 0.8f;

aitaskfade::aitaskfade(entity* object, const float &target, const bool &instant) :
	iaitask(object)
{
	m_target = target;
	m_instant = instant;
}

aitaskfade::~aitaskfade()
{
	delete m_driver;
}

void aitaskfade::start()
{
	m_driver = new driverslide(m_entity->getAlpha(), m_target, TIME_FADE);
}

void aitaskfade::update()
{
	if (m_instant)
	{
		m_entity->setAlpha(m_target);
	}
	else
	{
		m_entity->setAlpha(m_driver->update(framework::get()->getDelta()));
	}
}

bool aitaskfade::isComplete()
{
	return m_entity->getAlpha() == m_target;
}
