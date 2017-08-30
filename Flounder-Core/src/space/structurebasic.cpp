#include "structurebasic.hpp"

namespace flounder
{
	template<class t>
	structurebasic<t>::structurebasic() :
		ispatialstructure<t>(),
		m_objects(new std::vector<t>())
	{
	}

	template<class t>
	structurebasic<t>::~structurebasic()
	{
		delete m_objects;
	}

	template<class t>
	void structurebasic<t>::add(t object)
	{
		m_objects->push_back(object);
	}

	template<class t>
	void structurebasic<t>::remove(t object)
	{
		for (auto it = m_objects->begin(); it != m_objects->end(); ++it)
		{
			if (*it == object)
			{
				m_objects->erase(it);
				return;
			}
		}
	}

	template<class t>
	void structurebasic<t>::clear()
	{
		m_objects->clear();
	}

	template<class t>
	int structurebasic<t>::getSize()
	{
		return m_objects->size();
	}

	template<class t>
	std::vector<t> *structurebasic<t>::getAll()
	{
		return m_objects;
	}

	template<class t>
	std::vector<t> *structurebasic<t>::getAll(std::vector<t> *result)
	{
		for (auto value : m_objects)
		{
			result->push_back(value);
		}
	}

	template<class t>
	std::vector<t> *structurebasic<t>::queryInFrustum(frustum *range, std::vector<t> *result)
	{
		for (auto value : m_objects)
		{
			ispatialobject *object = static_cast<ispatialobject*>(value);

			if (object->getCollider() == nullptr || object->getCollider()->inFrustum(*range))
			{
				result->push_back(value);
			}
		}
	}

	template<class t>
	std::vector<t> *structurebasic<t>::queryInBounding(icollider *range, std::vector<t> *result)
	{
		for (auto value : m_objects)
		{
			ispatialobject *object = static_cast<ispatialobject*>(value);

			if (object->getCollider() == nullptr || range->intersects(*object->getCollider()) || range->contains(*object->getCollider()))
			{
				result->push_back(value);
			}
		}
	}

	template<class t>
	bool structurebasic<t>::contains(ispatialobject *object)
	{
		return std::find(m_objects->begin(), m_objects->end(), object) != m_objects->end();
	}
}
