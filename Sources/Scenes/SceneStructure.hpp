#pragma once

#include "Physics/Rigidbody.hpp"
#include "Entity.hpp"

namespace acid
{
/**
 * @brief Class that represents a  structure of spatial objects.
 */
class ACID_EXPORT SceneStructure :
	public NonCopyable
{
public:
	SceneStructure();

	Entity *GetEntity(const std::string &name) const;

	/**
	 * Creates a new entity.
	 * @return The newly created entity.
	 */
	Entity *CreateEntity();

	/**
	 * Creates a new entity from a prefab.
	 * @param filename The file to load the component data from.
	 * @return The newly created entity.
	 */
	Entity *CreateEntity(const std::string &filename);

	/**
	 * Adds a new object to the spatial structure.
	 * @param object The object to add.
	 */
	void Add(Entity *object);

	/**
	 * Adds a new object to the spatial structure.
	 * @param object The object to add.
	 */
	void Add(std::unique_ptr<Entity> object);

	/**
	 * Removes an object from the spatial structure.
	 * @param object The object to remove.
	 */
	void Remove(Entity *object);

	/**
	 * Moves an object to another spatial structure.
	 * @param object The object to move.
	 * @param structure The structure to move to.
	 */
	void Move(Entity *object, SceneStructure &structure);

	/**
	 * Removes all objects from the spatial structure.
	 */
	void Clear();

	/**
	 * Updates all of the entity.
	 */
	void Update();

	/**
	 * Gets the size of this structure.
	 * @return The structures size.
	 */
	uint32_t GetSize() const { return static_cast<uint32_t>(m_objects.size()); }

	/**
	 * Gets a set of all objects in the spatial structure.
	 * @return The list specified by of all objects.
	 */
	std::vector<Entity *> QueryAll();

	/**
	 * Gets a set of all objects in a spatial objects contained in a frustum.
	 * @param range The frustum range of space being queried.
	 * @return The list of all object in range.
	 */
	std::vector<Entity *> QueryFrustum(const Frustum &range);

	//std::vector<Entity *> QuerySphere(const Vector3 &centre, const Vector3 &radius);

	//std::vector<Entity *> QueryCube(const Vector3 &min, const Vector3 &max);

	/**
	 * Returns a set of all components of a type in the spatial structure.
	 * @tparam T The components type to get.
	 * @param allowDisabled If disabled components will be included in this query.
	 * @return The list specified by of all components that match the type.
	 */
	template<typename T>
	std::vector<T *> QueryComponents(bool allowDisabled = false)
	{
		std::vector<T *> components;

		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			for (const auto &component : (*it)->GetComponents<T>())
			{
				if (component && (component->IsEnabled() || allowDisabled))
				{
					components.emplace_back(component);
				}
			}
		}

		return components;
	}

	/**
	 * Gets the first component of a type found in the spatial structure.
	 * @tparam T The component type to get.
	 * @param allowDisabled If disabled components will be included in this query.
	 * @return The first component of the type found.
	 */
	template<typename T>
	T *GetComponent(bool allowDisabled = false)
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			auto component = (*it)->GetComponent<T>();

			if (component && (component->IsEnabled() || allowDisabled))
			{
				return component;
			}
		}

		return nullptr;
	}

	/**
	 * If the structure contains the object.
	 * @param object The object to check for.
	 * @return If the structure contains the object.
	 */
	bool Contains(Entity *object);

private:
	std::vector<std::unique_ptr<Entity>> m_objects;
};
}
