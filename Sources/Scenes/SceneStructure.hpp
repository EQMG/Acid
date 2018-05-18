#pragma once

#include <vector>
#include <algorithm>
#include "ISpatialStructure.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/Component.hpp"
#include "Physics/Rigidbody.hpp"

namespace fl
{
	/// <summary>
	/// A structure of spatial objects for a 3D space.
	/// </summary>
	class FL_EXPORT SceneStructure :
		public ISpatialStructure
	{
	private:
		std::vector<GameObject *> *m_objects;
	public:
		/// <summary>
		/// Creates a new basic structure.
		/// </summary>
		SceneStructure();

		/// <summary>
		/// Deconstructor for the basic structure.
		/// </summary>
		~SceneStructure();

		void Add(GameObject *object) override;

		void Remove(GameObject *object) override;

		void Clear() override;

		unsigned int GetSize() override { return m_objects->size(); }

		std::vector<GameObject *> *GetAll() override { return m_objects; }

		/// <summary>
		/// Returns a set of all components of a type in the spatial structure.
		/// </summary>
		/// <param name="result"> The list to store the data into.</param>
		/// <returns> The list specified by of all components that match the type. </returns>
		template<typename T>
		std::vector<T *> *QueryComponents(std::vector<T *> *result)
		{
			for (auto it = m_objects->begin(); it != m_objects->end(); ++it)
			{
				auto gameObject = static_cast<GameObject *>(*it);
				auto component = gameObject->GetComponent<T>();

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
		template<typename T>
		T *GetComponent()
		{
			for (auto it = m_objects->begin(); it != m_objects->end(); ++it)
			{
				auto gameObject = static_cast<GameObject *>(*it);
				auto component = gameObject->GetComponent<T>();

				if (component != nullptr)
				{
					return component;
				}
			}

			return nullptr;
		}

		std::vector<GameObject *> *QueryAll(std::vector<GameObject *> *result) override;

		std::vector<GameObject *> *QueryFrustum(Frustum *range, std::vector<GameObject *> *result) override;

		std::vector<GameObject *> *QueryBounding(ICollider *range, std::vector<GameObject *> *result) override;

		bool Contains(GameObject *object) override;
	};
}
