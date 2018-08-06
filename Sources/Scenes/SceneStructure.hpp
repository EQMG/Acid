#pragma once

#include <algorithm>
#include <vector>
#include "Objects/GameObject.hpp"
#include "Objects/IComponent.hpp"
#include "Physics/Rigidbody.hpp"
#include "ISpatialStructure.hpp"

namespace acid
{
	/// <summary>
	/// A structure of spatial objects for a 3D space.
	/// </summary>
	class ACID_EXPORT SceneStructure :
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

		bool Remove(GameObject *object) override;

		void Clear() override;

		uint32_t GetSize() override { return m_objects.size(); }

		std::vector<GameObject *> GetAll() override { return m_objects; }

		std::vector<GameObject *> QueryAll() override;

		std::vector<GameObject *> QueryFrustum(const Frustum &range) override;

	//	std::vector<GameObject *> QueryBounding(Collider *range) override;

		/// <summary>
		/// Returns a set of all components of a type in the spatial structure.
		/// </summary>
		/// <param name="allowDisabled"> If disabled components will be included in this query. </param>
		/// <returns> The list specified by of all components that match the type. </returns>
		template<typename T>
		std::vector<T *> QueryComponents(const bool &allowDisabled = false)
		{
			auto result = std::vector<T *>();

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
		T *GetComponent(const bool &allowDisabled = false)
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

		bool Contains(GameObject *object) override;
	};
}
