#include "structurebasic.h"

namespace flounder
{
	template<typename t>
	structurebasic<t>::structurebasic() :
		ispatialstructure()
	{
		m_objects = new std::vector<t>();
	}

	template<typename t>
	structurebasic<t>::~structurebasic()
	{
		delete m_objects;
	}

	template<typename t>
	void structurebasic<t>::add(t *object)
	{
		m_objects->push_back(object);
	}

	template<typename t>
	void structurebasic<t>::remove(t *object)
	{
		m_objects->erase(object);
	}

	template<typename t>
	void structurebasic<t>::clear()
	{
		m_objects->clear();
	}

	template<typename t>
	int structurebasic<t>::getSize()
	{
		return m_objects->size();
	}

	template<typename t>
	std::vector<t *> structurebasic<t>::getAll()
	{
		return m_objects;
	}

	template<typename t>
	std::vector<t*> structurebasic<t>::getAll(std::vector<t*> *result)
	{
		for (t value : m_objects)
		{
			result->push_back(value);
		}
	}

	template<typename t>
	std::vector<t*> structurebasic<t>::queryInFrustum(frustum *range, std::vector<t *> *result)
	{
		for (t value : m_objects)
		{
			ispatialobject *object = static_cast<ispatialobject*>(value);

			if (object->getCollider() == NULL || object->getCollider()->inFrustum(range))
			{
				result->push_back(value);
			}
		}
	}

	template<typename t>
	std::vector<t*> structurebasic<t>::queryInBounding(icollider *range, std::vector<t *> *result)
	{
		for (t value : m_objects)
		{
			ispatialobject *object = static_cast<ispatialobject*>(value);

			if (object->getCollider() == NULL || range->intersects(object->getCollider()) || range->contains(object->getCollider()))
			{
				result->push_back(value);
			}
		}
	}

	template<typename t>
	bool structurebasic<t>::contains(ispatialobject *object)
	{
		return std::find(m_objects->begin(), m_objects->end(), object) != m_objects->end();
	}
}
