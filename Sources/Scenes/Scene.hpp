#pragma once

#include "Helpers/Delegate.hpp"
#include "Helpers/TypeInfo.hpp"
#include "Holders/ComponentHolder.hpp"
#include "Holders/EntityPool.hpp"
#include "Holders/SystemHolder.hpp"
#include "Camera.hpp"
#include "Entity.inl"
#include "System.hpp"

namespace acid {
class ACID_EXPORT Scene : public virtual Observer {
	friend class Scenes;
	friend class Entity;
	friend class System;
public:
	/**
	 * Creates a new scene.
	 * @param camera The scenes camera.
	 */
	explicit Scene(std::unique_ptr<Camera> &&camera);

	virtual ~Scene();

	/**
	 * Run when switching to this scene from another.
	 */
	virtual void Start() = 0;

	/**
	 * Run when updating the scene.
	 */
	virtual void Update() = 0;

	/**
	 * Gets if the scene is paused.
	 * @return If the scene is paused.
	 */
	virtual bool IsPaused() const = 0;

	/**
	 * Gets the current camera object.
	 * @return The current camera.
	 */
	Camera *GetCamera() const { return m_camera.get(); }

	// TODO: Remove
	template<typename T>
	std::vector<T *> GetComponents() const;

	/**
	 * Checks whether a System exists or not.
	 * @tparam T The System type.
	 * @return If the Scene has the System.
	 */
	template<typename T>
	bool HasSystem() const;

	/**
	 * Gets a System.
	 * @tparam T The System type.
	 * @return The System.
	 */
	template<typename T>
	T *GetSystem() const;

	/**
	 * Adds a System.
	 * @tparam T The System type.
	 * @tparam Args The constructor arg types.
	 * @param priority The System priority.
	 * @param args The constructor args.
	 * @return The System.
	 */
	template<typename T, typename... Args>
	T *AddSystem(std::size_t priority = 0, Args &&...args);

	/**
	 * Removes a System.
	 * @tparam T The System type.
	 */
	template<typename T>
	void RemoveSystem();

	/**
	 * Removes all Systems.
	 */
	void RemoveAllSystems();

	/**
	 * Creates a new Entity.
	 * @return The Entity.
	 */
	Entity CreateEntity();

	/**
	 * Creates a new named Entity.
	 * @param name The Entity name.
	 * @return The Entity.
	 */
	Entity CreateEntity(const std::string &name);

	/**
	 * Creates a new Entity from a prefab.
	 * @param filename The Entity prefab file.
	 * @return The Entity.
	 */
	Entity CreatePrefabEntity(const std::string &filename);

	/**
	 * Gets a Entity by ID.
	 * @param id The Entity ID.
	 * @return The entity.
	 */
	std::optional<Entity> GetEntity(Entity::Id id) const;

	/**
	 * Gets a Entity by name.
	 * @param name The Entity name.
	 * @return The entity.
	 */
	std::optional<Entity> GetEntity(const std::string &name) const;

	/**
	 * Gets a Entity name.
	 * @param id The Entity ID.
	 * @return The Entity name.
	 */
	std::string GetEntityName(Entity::Id id) const;

	/**
	 * Gets whether the Entity is enabled or not.
	 * @param id The Entity ID.
	 * @return If the Entity is enabled.
	 */
	bool IsEntityEnabled(Entity::Id id) const;

	/**
	 * Enables a Entity.
	 * @param id The Entity ID.
	 */
	void EnableEntity(Entity::Id id);

	/**
	 * Disables a Entity.
	 * @param id The Entity ID.
	 */
	void DisableEntity(Entity::Id id);

	/**
	 * Gets whether an Entity is valid or not.
	 * @param id The Entity ID.
	 * @return If the Entity is valid.
	 */
	bool IsEntityValid(Entity::Id id) const;

	/**
	 * Removes a Entity.
	 * @param id The Entity ID.
	 */
	void RemoveEntity(Entity::Id id);

	/**
	 * Refreshes the Entity and Systems list.
	 * @param id The Entity ID.
	 */
	void RefreshEntity(Entity::Id id);

	/**
	 * Removes all Entities.
	 */
	void RemoveAllEntities();

	/**
	 * Updates the Scene.
	 * @param delta The time delta between the last update.
	 */
	void Update(float delta);

	/**
	 * Clears the Scene by removing all Systems and Entities.
	 */
	void Clear();

private:
	class EntityAttributes {
	public:
		// Entity.
		Entity m_entity;

		// Is this Entity enabled.
		bool m_enabled = true;

		// Is this Entity valid (hasn't been removed).
		bool m_valid = true;

		// Entity name.
		std::optional<std::string> m_name;

		// The Systems this Entity is attached.
		std::vector<TypeId> m_systems;
	};

	class EntityAction {
	public:
		enum class Action {
			Enable, Disable, Remove, Refresh
		};

		EntityAction(Entity::Id id, Action action) :
			id(id),
			action(action) {
		}

		// Entity ID.
		Entity::Id id;

		// Action to perform on this Entity.
		Action action;
	};

	enum class EntityAttachStatus {
		Attached, AlreadyAttached, Detached, NotAttached
	};

	/**
	 * Update the Entity actions within the World.
	 */
	void UpdateEntities();

	/**
	 * Executes an action.
	 * @param action The action to execute.
	 */
	void ExecuteAction(const EntityAction &action);

	/**
	 * Adds the Entity to the Systems it meets the requirements.
	 * @param id The Entity ID.
	 */
	void ActionEnable(Entity::Id id);

	/**
	 * Removes the Entity from the Systems it meets the requirements.
	 * @param id The Entity ID.
	 */
	void ActionDisable(Entity::Id id);

	/**
	 * Removes the Entity data from the World.
	 * @param id The Entity ID.
	 */
	void ActionRemove(Entity::Id id);

	/**
	 * Attaches the Entity to the Systems it meets the requirements or detach it from the Systems it does not meet the requirements anymore.
	 * Used after AddComponent and RemoveComponent.
	 * @param id The Entity ID.
	 */
	void ActionRefresh(Entity::Id id);

	/**
	 * Extends the Entity and Component arrays.
	 * @param size The new size.
	 */
	void Extend(std::size_t size);

	/**
	 * Checks the requirements the Entity meets for each Systems.
	 * @param system The System.
	 * @param systemId The System ID.
	 * @param id The Entity ID.
	 * @return The attachment status.
	 */
	EntityAttachStatus TryEntityAttach(System &system, TypeId systemId, Entity::Id id);

	// If this scene object has been started yet.
	bool m_started = false;

	// The camera of this scene.
	std::unique_ptr<Camera> m_camera;

	// List of all Entities.
	std::vector<EntityAttributes> m_entities;

	// List of Entities that have been modified.
	std::vector<EntityAction> m_actions;

	// List of all Entity names, associated to their Entities, for faster search.
	std::unordered_map<std::string, Entity::Id> m_names;

	// List of all Components of all Entities of the Scene.
	ComponentHolder m_components;

	// List of all Systems of the Scene.
	SystemHolder m_systems;

	// List of all System waiting to be started.
	std::vector<System *> m_newSystems;

	// Entity ID Pool.
	EntityPool m_pool;
};
}

#include "Scene.inl"
