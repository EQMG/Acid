#include "StructureBasic.hpp"

namespace Flounder
{
	template<class t>
	StructureBasic<t>::StructureBasic() :
		ISpatialStructure<t>(),
		m_objects(new std::vector<t>())
	{
	}

	template<class t>
	StructureBasic<t>::~StructureBasic()
	{
		delete m_objects;
	}

	template<class t>
	void StructureBasic<t>::Add(t object)
	{
		m_objects->push_back(object);
	}

	template<class t>
	void StructureBasic<t>::Remove(t object)
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
	void StructureBasic<t>::Clear()
	{
		m_objects->Clear();
	}

	template<class t>
	int StructureBasic<t>::GetSize()
	{
		return m_objects->size();
	}

	template<class t>
	std::vector<t> *StructureBasic<t>::GetAll()
	{
		return m_objects;
	}

	template<class t>
	std::vector<t> *StructureBasic<t>::QueryAll(std::vector<t> *result)
	{
		for (auto value : m_objects)
		{
			result->push_back(value);
		}
	}

	template<class t>
	std::vector<t> *StructureBasic<t>::QueryFrustum(Frustum *range, std::vector<t> *result)
	{
		for (auto value : m_objects)
		{
			ISpatialObject *object = static_cast<ISpatialObject*>(value);

			if (object->GetCollider() == nullptr || object->GetCollider()->InFrustum(*range))
			{
				result->push_back(value);
			}
		}
	}

	template<class t>
	std::vector<t> *StructureBasic<t>::QueryBounding(ICollider *range, std::vector<t> *result)
	{
		for (auto value : m_objects)
		{
			ISpatialObject *object = static_cast<ISpatialObject*>(value);

			if (object->GetCollider() == nullptr || range->Intersects(*object->GetCollider()) || range->Contains(*object->GetCollider()))
			{
				result->push_back(value);
			}
		}
	}

	template<class t>
	bool StructureBasic<t>::Contains(ISpatialObject *object)
	{
		return std::find(m_objects->begin(), m_objects->end(), object) != m_objects->end();
	}
}
