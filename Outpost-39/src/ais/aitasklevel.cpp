#include "aitasklevel.h"

aitasklevel::aitasklevel(entity* object, const int &level) :
	iaitask(object)
{
	m_level = level;
}

aitasklevel::~aitasklevel()
{
}

void aitasklevel::update()
{
	m_entity->getPosition()->set(0.5f, levelToPosition(m_level));

	// Walk anim.
	m_entity->setSelectedRow(0);
}

bool aitasklevel::isComplete()
{
	return true;
}
