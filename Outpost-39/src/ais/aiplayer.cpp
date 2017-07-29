#include "aiplayer.h"

aiplayer::aiplayer(texture *texture, const std::string &name)
{
	m_entity = new entity(vector2(0.0f, 0.5f), vector2(0.08f, 0.08f), texture, 1);
	m_entity->addComponent([&](entity *object)
	{
		if (m_trackCentre)
		{
			traceX(0.5f);

			if (isAtCentre())
			{
				m_trackCentre = false;
			}

			// Walk anim.
			object->setSelectedRow(1);
		}
		else if (m_sleeping)
		{
			if (!isAtBed())
			{
				//	if (m_bedXPos != -1.0f)
				//	{
				traceX(m_bedXPos);
				//	}
			}
			else
			{
				// m_bedXPos = -1.0f;
				// GO to seeping state.
				object->setSelectedRow(4);
			}
		}
	});
	entities::get()->add(m_entity);
	setLevel(0);

	m_name = name;
	m_trackCentre = false;
	m_sleeping = false;
	m_bedXPos = -1.0f;
	m_adventuting = false;
	m_dead = false;
}

aiplayer::~aiplayer()
{
	delete m_entity;
}

bool aiplayer::isAtCentre()
{
	return maths::almostEqual(m_entity->getPosition()->m_x, 0.5f, 0.007f);
}

void aiplayer::goToCentre()
{
	m_trackCentre = true;
}

int aiplayer::getLevel()
{
	return (0.842f - m_entity->getPosition()->m_y) / 0.134f;
}

void aiplayer::setLevel(const int &level)
{
	m_entity->getPosition()->m_y = 0.842f - (level * 0.134f);
}

bool aiplayer::isAtBed()
{
	return maths::almostEqual(m_entity->getPosition()->m_x, m_bedXPos, 0.007f);
}

void aiplayer::setBedTarget(const float &x)
{
	m_bedXPos = x;
	m_sleeping = true;
}

void aiplayer::cancelSleep()
{
	m_bedXPos = -1.0f;
	m_sleeping = false;
}

void aiplayer::traceX(const float &x)
{
	bool left = (m_entity->getPosition()->m_x - x) <= 0.0f;
	const float track_rate = 0.07f;

	if (left)
	{
		m_entity->getPosition()->m_x += track_rate * framework::get()->getDelta();
		m_entity->setFlipTexture(true);
	}
	else
	{
		m_entity->getPosition()->m_x -= track_rate * framework::get()->getDelta();
		m_entity->setFlipTexture(false);
	}
}
