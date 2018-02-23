﻿#pragma once

#include <vector>
#include <algorithm>
#include "ISpatialStructure.hpp"
#include "../../Objects/GameObject.hpp"
#include "../../Objects/Component.hpp"
#include "../../Physics/Collider.hpp"

namespace Flounder
{
	/// <summary>
	/// A structure of spatial objects for a 3D space.
	/// </summary>
	template<typename T>
	class F_EXPORT StructureBasic :
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

		unsigned int GetSize() override { return m_objects->size(); }

		std::vector<T> *GetAll() override { return m_objects; }

		/// <summary>
		/// Returns a set of all components of a type in the spatial structure.
		/// </summary>
		/// <param name="result"> The list to store the data into.</param>
		/// <returns> The list specified by of all components that match the type. </returns>
		template<typename K>
		std::vector<K*> *QueryComponents(std::vector<K*> *result)
		{
			for (auto object : *GetAll())
			{
				auto gameObject = static_cast<GameObject *>(object);
				auto component = gameObject->GetComponent<K>();

				if (component != nullptr)
				{
					result->push_back(component);
				}
			}

			return result;
		}

		/// <summary>
		/// Returns the first component of a type found in the spatial structure.
		/// </summary>
		/// <returns> The first component of the type found. </returns>
		template<typename K>
		K *GetComponent()
		{
			for (auto object : *GetAll())
			{
				auto gameObject = static_cast<GameObject *>(object);
				auto component = gameObject->GetComponent<K>();

				if (component != nullptr)
				{
					return component;
				}
			}

			return nullptr;
		}

		std::vector<T> *QueryAll(std::vector<T> *result) override
		{
			for (auto object : *m_objects)
			{
				result->push_back(object);
			}

			return result;
		}

		std::vector<T> *QueryFrustum(Frustum *range, std::vector<T> *result) override
		{
			for (auto object : *m_objects)
			{
				auto gameObject = static_cast<GameObject *>(object);
				auto collider = gameObject->GetComponent<Collider>();

				if (collider == nullptr || collider->InFrustum(*range))
				{
					result->push_back(object);
				}
			}

			return result;
		}

		std::vector<T> *QueryBounding(Collider *range, std::vector<T> *result) override
		{
			for (auto object : *m_objects)
			{
				auto gameObject = static_cast<GameObject *>(object);
				auto collider = gameObject->GetComponent<Collider>();

				if (collider == nullptr || range->Intersects(*collider).IsIntersection() || range->Contains(*collider))
				{
					result->push_back(object);
				}
			}

			return result;
		}

		bool Contains(GameObject *object) override
		{
			return std::find(m_objects->begin(), m_objects->end(), object) != m_objects->end();
		}
	};
}
