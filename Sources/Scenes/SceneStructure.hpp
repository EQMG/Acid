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
		std::vector<std::unique_ptr<GameObject>> m_objects;
	public:
		/// <summary>
		/// Creates a new scene structure.
		/// </summary>
		SceneStructure();

		SceneStructure(const SceneStructure&) = delete;

		SceneStructure& operator=(const SceneStructure&) = delete;

		void Add(GameObject *object) override;

		void Add(std::unique_ptr<GameObject> object) override;

		bool Remove(GameObject *object) override;

		bool Move(GameObject *object, ISpatialStructure *structure) override;

		void Clear() override;

		void Update() override;

		uint32_t GetSize() override { return static_cast<uint32_t>(m_objects.size()); }

		std::vector<GameObject *> QueryAll() override;

		std::vector<GameObject *> QueryFrustum(const Frustum &range) override;

	//	std::vector<GameObject *> QuerySphere(const Vector3 &centre, const Vector3 &radius) override;

	//	std::vector<GameObject *> QueryCube(const Vector3 &min, const Vector3 &max) override;

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
