#pragma once

#include <vector>
#include <algorithm>
#include "ISpatialStructure.hpp"
#include "Objects/GameObject.hpp"
#include "Objects/IComponent.hpp"
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
		std::vector<GameObject *> m_objects;
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

		unsigned int GetSize() override { return m_objects.size(); }

		std::vector<GameObject *> GetAll() override { return m_objects; }

		/// <summary>
		/// Returns a set of all components of a type in the spatial structure.
		/// </summary>
		/// <param name="allowDisabled"> If disabled components will be included in this query. </param>
		/// <returns> The list specified by of all components that match the type. </returns>
		template<typename T>
		std::vector<std::shared_ptr<T>> QueryComponents(const bool &allowDisabled = false)
		{
			auto result = std::vector<std::shared_ptr<T>>();

			for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
			{
				if ((*it)->IsRemoved())
				{
					continue;
				}

				auto component = (*it)->GetComponent<T>();

				if (component != nullptr && (component->IsEnabled() || allowDisabled))
				{
					result.emplace_back(component);
				}
			}

			return result;
		}

		/// <summary>
		/// Returns the first component of a type found in the spatial structure.
		/// </summary>
		/// <param name="allowDisabled"> If disabled components will be included in this query. </param>
		/// <returns> The first component of the type found. </returns>
		template<typename T>
		std::shared_ptr<T> GetComponent(const bool &allowDisabled = false)
		{
			for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
			{
				if ((*it)->IsRemoved())
				{
					continue;
				}

				auto component = (*it)->GetComponent<T>();

				if (component != nullptr && (component->IsEnabled() || allowDisabled))
				{
					return component;
				}
			}

			return nullptr;
		}

		std::vector<GameObject *> QueryAll() override;

		std::vector<GameObject *> QueryFrustum(const Frustum &range) override;

		std::vector<GameObject *> QueryBounding(ICollider *range) override;

		bool Contains(GameObject *object) override;
	};
}
