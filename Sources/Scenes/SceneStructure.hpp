#pragma once

#include <algorithm>
#include <mutex>
#include <vector>
#include "Physics/Rigidbody.hpp"
#include "Component.hpp"
#include "Entity.hpp"

namespace acid
{
	/// <summary>
	/// A structure of spatial objects for a scene.
	/// </summary>
	class ACID_EXPORT SceneStructure :
		public NonCopyable
	{
	public:
		/// <summary>
		/// Creates a new scene structure.
		/// </summary>
		SceneStructure();

		/// <summary>
		/// Creates a new entity that starts in this structure.
		/// </summary>
		/// <param name="transform"> The objects initial world position, rotation, and scale. </param>
		/// <returns> The newly created entity. </returns>
		Entity *CreateEntity(const Transform &transform);

		/// <summary>
		/// Creates a new entity from a prefab that starts in this structure.
		/// </summary>
		/// <param name="filename"> The file to load the component data from. </param>
		/// <param name="transform"> The objects initial world position, rotation, and scale. </param>
		/// <returns> The newly created entity. </returns>
		Entity *CreateEntity(const std::string &filename, const Transform &transform);

		/// <summary>
		/// Adds a new object to the spatial structure.
		/// </summary>
		/// <param name="object"> The object to add. </param>
		void Add(Entity *object);

		/// <summary>
		/// Adds a new object to the spatial structure.
		/// </summary>
		/// <param name="object"> The object to add. </param>
		void Add(std::unique_ptr<Entity> object);

		/// <summary>
		/// Removes an object from the spatial structure.
		/// </summary>
		/// <param name="object"> The object to remove. </param>
		void Remove(Entity *object);

		/// <summary>
		/// Moves an object to another spatial structure.
		/// </summary>
		/// <param name="object"> The object to remove. </param>
		/// <param name="structure"> The structure to move to. </param>
		void Move(Entity *object, SceneStructure &structure);

		/// <summary>
		/// Removes all objects from the spatial structure..
		/// </summary>
		void Clear();

		/// <summary>
		/// Updates all of the entity.
		/// </summary>
		void Update();

		/// <summary>
		/// Gets the size of this structure.
		/// </summary>
		/// <returns> The structures size. </returns>
		uint32_t GetSize() const { return static_cast<uint32_t>(m_objects.size()); }

		/// <summary>
		/// Returns a set of all objects in the spatial structure.
		/// </summary>
		/// </param>
		/// <returns> The list specified by of all objects. </returns>
		std::vector<Entity *> QueryAll();

		/// <summary>
		/// Returns a set of all objects in a spatial objects contained in a frustum.
		/// </summary>
		/// <param name="range"> The frustum range of space being queried. </param>
		/// </param>
		/// <returns> The list of all object in range. </returns>
		std::vector<Entity *> QueryFrustum(const Frustum &range);

		//	std::vector<Entity *> QuerySphere(const Vector3 &centre, const Vector3 &radius);

		//	std::vector<Entity *> QueryCube(const Vector3 &min, const Vector3 &max);

		/// <summary>
		/// Returns a set of all components of a type in the spatial structure.
		/// </summary>
		/// <param name="allowDisabled"> If disabled components will be included in this query. </param>
		/// <returns> The list specified by of all components that match the type. </returns>
		template<typename T>
		std::vector<T *> QueryComponents(const bool &allowDisabled = false)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			std::vector<T *> result = {};

			for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
			{
				auto components = (*it)->GetComponents<T>();

				for (const auto &component : components)
				{
					if (component != nullptr && (component->IsEnabled() || allowDisabled))
					{
						result.emplace_back(component);
					}
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
			std::lock_guard<std::mutex> lock(m_mutex);

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

		/// <summary>
		/// If the structure contains the object.
		/// </summary>
		/// <param name="object"> The object to check for.
		/// </param>
		/// <returns> If the structure contains the object. </returns>
		bool Contains(Entity *object);

	private:
		std::mutex m_mutex;
		std::vector<std::unique_ptr<Entity>> m_objects;
	};
}
