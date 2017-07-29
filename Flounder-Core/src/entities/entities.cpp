#include "entities.h"

namespace flounder
{
	entities::entities() :
		imodule()
	{
		m_entities = new std::vector<entity*>();
	}

	entities::~entities()
	{
		delete m_entities;
	}

	void entities::add(entity *object)
	{
		m_entities->push_back(object);
	}

	void entities::remove(entity *object)
	{
		for (std::vector<entity*>::iterator it = m_entities->begin(); it != m_entities->end(); ++it)
		{
			if (*it == object)
			{
				m_entities->erase(it);
				return;
			}
		}
	}

	void entities::update()
	{
		for (entity *object : *m_entities)
		{
			object->update();
		}
	}
}
