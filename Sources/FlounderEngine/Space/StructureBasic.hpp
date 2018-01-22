#pragma once

#include <vector>
#include <algorithm>
#include "ISpatialStructure.hpp"

namespace Flounder
{
	/// <summary>
	/// A structure of spatial objects for a 3D space.
	/// </summary>
	template<typename T>
	class StructureBasic :
		public ISpatialStructure<T>
	{
	private:
		std::vector<T> *m_objects;
	public:
		/// <summary>
		/// Creates a new basic structure.
		/// </summary>
		StructureBasic() :
			ISpatialStructure<T>(),
			m_objects(new std::vector<T>())
		{
		}

		/// <summary>
		/// Deconstructor for the basic structure.
		/// </summary>
		~StructureBasic()
		{
			delete m_objects;
		}

		void Add(T object) override
		{
			m_objects->push_back(object);
		}

		void Remove(T object) override
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

		void Clear() override
		{
			m_objects->clear();
		}

		int GetSize() override { return m_objects->size(); }

		std::vector<T> *GetAll() override { return m_objects; }

		std::vector<T> *QueryAll(std::vector<T> *result) override
		{
			for (auto value : *m_objects)
			{
				result->push_back(value);
			}

			return result;
		}

		std::vector<T> *QueryFrustum(Frustum *range, std::vector<T> *result) override
		{
			for (auto value : *m_objects)
			{
				ISpatialObject *object = static_cast<ISpatialObject *>(value);

				if (object->GetCollider() == nullptr || object->GetCollider()->InFrustum(*range))
				{
					result->push_back(value);
				}
			}

			return result;
		}

		std::vector<T> *QueryBounding(ICollider *range, std::vector<T> *result) override
		{
			for (auto value : *m_objects)
			{
				ISpatialObject *object = static_cast<ISpatialObject *>(value);

				if (object->GetCollider() == nullptr || range->Intersects(*object->GetCollider()).IsIntersection() ||
					range->Contains(*object->GetCollider()))
				{
					result->push_back(value);
				}
			}

			return result;
		}

		bool Contains(ISpatialObject *object) override
		{
			return std::find(m_objects->begin(), m_objects->end(), object) != m_objects->end();
		}
	};
}
