#include "entity.h"

namespace flounder
{
	entity::entity(ispatialstructure<entity*> *structure, const vector3 &position, const vector3 &rotation)
	{
		m_structure = structure;

		m_components = new std::vector<icomponent*>();

		m_position = new vector3(position);
		m_rotation = new vector3(rotation);

		m_removed = false;

		if (m_structure != nullptr)
		{
			m_structure->add(this);
		}
	}

	entity::~entity()
	{
		remove();

		for (icomponent *c : *m_components)
		{
			delete c;
		}

		delete m_components;

		delete m_position;
		delete m_rotation;
	}

	void entity::update()
	{
		for (icomponent *c : *m_components)
		{
			c->update();
		}
	}

	void entity::moveStructure(ispatialstructure<entity*> *structure)
	{
		m_structure->remove(this);
		structure->add(this);
		m_structure = structure;
	}

	void entity::addComponent(icomponent *component)
	{
		m_components->push_back(component);
		component->setEntity(this);
	}

	void entity::removeComponent(icomponent *component)
	{
		for (std::vector<icomponent*>::iterator it = m_components->begin(); it != m_components->end(); ++it)
		{
			if (*it == component)
			{
				component->setEntity(nullptr);
				delete component;
				m_components->erase(it);
				return;
			}
		}
	}

	template<class t>
	t entity::getComponent()
	{
		for (icomponent *c : *m_components)
		{
			t casted = dynamic_cast<t>(c);

			if (casted != nullptr)
			{
				return casted;
			}
		}

		return nullptr;
	}

	icollider *entity::getCollider()
	{
		return nullptr;
	}

	void entity::remove()
	{
		m_removed = true;
		m_structure->remove(this);
	}
}
