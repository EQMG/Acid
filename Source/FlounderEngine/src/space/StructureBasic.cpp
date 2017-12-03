#include "StructureBasic.hpp"

namespace Flounder
{
	template<typename T>
	StructureBasic<T>::StructureBasic() :
		ISpatialStructure<T>(),
		m_objects(new std::vector<T>())
	{
	}

	template<typename T>
	StructureBasic<T>::~StructureBasic()
	{
		delete m_objects;
	}

	template<typename T>
	void StructureBasic<T>::Add(T object)
	{
		m_objects->push_back(object);
	}

	template<typename T>
	void StructureBasic<T>::Remove(T object)
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

	template<typename T>
	void StructureBasic<T>::Clear()
	{
		m_objects->Clear();
	}

	template<typename T>
	int StructureBasic<T>::GetSize()
	{
		return m_objects->size();
	}

	template<typename T>
	std::vector<T> *StructureBasic<T>::GetAll()
	{
		return m_objects;
	}

	template<typename T>
	std::vector<T> *StructureBasic<T>::QueryAll(std::vector<T> *result)
	{
		for (auto value : m_objects)
		{
			result->push_back(value);
		}

		return result;
	}

	template<typename T>
	std::vector<T> *StructureBasic<T>::QueryFrustum(Frustum *range, std::vector<T> *result)
	{
		for (auto value : m_objects)
		{
			ISpatialObject *object = static_cast<ISpatialObject*>(value);

			if (object->GetCollider() == nullptr || object->GetCollider()->InFrustum(*range))
			{
				result->push_back(value);
			}
		}

		return result;
	}

	template<typename T>
	std::vector<T> *StructureBasic<T>::QueryBounding(ICollider *range, std::vector<T> *result)
	{
		for (auto value : m_objects)
		{
			ISpatialObject *object = static_cast<ISpatialObject*>(value);

			if (object->GetCollider() == nullptr || range->Intersects(*object->GetCollider()) || range->Contains(*object->GetCollider()))
			{
				result->push_back(value);
			}
		}

		return result;
	}

	template<typename T>
	bool StructureBasic<T>::Contains(ISpatialObject *object)
	{
		return std::find(m_objects->begin(), m_objects->end(), object) != m_objects->end();
	}
}
