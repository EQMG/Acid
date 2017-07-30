#include "aitasktargetx.h"

const float aitasktargetx::SPEED = 0.07f;
const float aitasktargetx::ERROR = 0.007f;

aitasktargetx::aitasktargetx(entity* object, const float &targetX) :
	iaitask(object)
{
	m_targetX = targetX;
}

aitasktargetx::~aitasktargetx()
{
}

void aitasktargetx::start()
{
}

void aitasktargetx::update()
{
	bool left = (m_entity->getPosition()->m_x - m_targetX) <= 0.0f;

	if (left)
	{
		m_entity->getPosition()->m_x += SPEED * framework::get()->getDelta();
		m_entity->setFlipX(true);
	}
	else
	{
		m_entity->getPosition()->m_x -= SPEED * framework::get()->getDelta();
		m_entity->setFlipX(false);
	}

	// Walk anim.
	m_entity->setSelectedRow(0);
}

bool aitasktargetx::isComplete()
{
	return maths::almostEqual(m_entity->getPosition()->m_x, m_targetX, ERROR);
}
