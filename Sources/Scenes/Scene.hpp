#pragma once

#include "Camera.hpp"
#include "EntityHolder.hpp"
#include "SystemHolder.hpp"

namespace acid {
/**
 * @brief Class that is used to represent a scene.
 */
class ACID_EXPORT Scene : public virtual rocket::trackable {
	friend class Scenes;
public:
	/**
	 * Creates a new scene.
	 * @param camera The scenes camera.
	 */
	explicit Scene(std::unique_ptr<Camera> &&camera);
	virtual ~Scene() = default;

	/**
	 * Run when switching to this scene from another.
	 */
	virtual void Start() = 0;

	/**
	 * Run when updating the scene.
	 */
	virtual void Update();

	/**
	 * Checks whether a System exists or not.
	 * @tparam T The System type.
	 * @return If the System exists.
	 */
	template<typename T>
	bool HasSystem() const {
		return systems.Has<T>();
	}

	/**
	 * Gets a System.
	 * @tparam T The System type.
	 * @return The System.
	 */
	template<typename T>
	T *GetSystem() const {
		return systems.Get<T>();
	}

	/**
	 * Adds a System.
	 * @tparam T The System type.
	 * @tparam Args The constructor arg types.
	 * @param args The constructor args.
	 * @return The System.
	 */
	template<typename T, typename... Args>
	void AddSystem(Args &&...args) {
		systems.Add<T>(std::make_unique<T>(std::forward<Args>(args)...));
	}

	/**
	 * Removes a System.
	 * @tparam T The System type.
	 */
	template<typename T>
	void RemoveSystem() {
		systems.Remove<T>();
	}

	/**
	 * Removes all Systems.
	 */
	void ClearSystems();

	/**
	 * Gets a Entity by name.
	 * @param name The Entity name.
	 * @return The entity.
	 */
	Entity *GetEntity(const std::string &name) const;

	/**
	 * Creates a new entity.
	 * @return The Entity.
	 */
	Entity *CreateEntity();

	/**
	 * Creates a new entity from a prefab.
	 * @param filename The file to load the component data from.
	 * @return The Entity.
	 */
	Entity *CreatePrefabEntity(const std::string &filename);

	/**
	 * Gets a set of all objects in the spatial structure.
	 * @return The list specified by of all objects.
	 */
	std::vector<Entity *> QueryAllEntities();
	
	/**
	 * Gets the first component of a type found in the spatial structure.
	 * @tparam T The component type to get.
	 * @param allowDisabled If disabled components will be included in this query.
	 * @return The first component of the type found.
	 */
	template<typename T>
	T *GetComponent(bool allowDisabled = false) {
		return entities.GetComponent<T>(allowDisabled);
	}

	/**
	 * Returns a set of all components of a type in the spatial structure.
	 * @tparam T The components type to get.
	 * @param allowDisabled If disabled components will be included in this query.
	 * @return The list specified by of all components that match the type.
	 */
	template<typename T>
	std::vector<T *> QueryComponents(bool allowDisabled = false) {
		return entities.QueryComponents<T>(allowDisabled);
	}

	/**
	 * Removes all Entities.
	 */
	void ClearEntities();

	/**
	 * Gets the current camera object.
	 * @return The current camera.
	 */
	Camera *GetCamera() const { return camera.get(); }

	/**
	 * Sets the current camera to a new camera.
	 * @param camera The new camera.
	 */
	void SetCamera(Camera *camera) { this->camera.reset(camera); }

	/**
	 * Gets if the scene is paused.
	 * @return If the scene is paused.
	 */
	virtual bool IsPaused() const = 0;

private:
	/// If this scene object has been started yet.
	bool started = false;

	/// List of all Systems of the Scene.
	SystemHolder systems;
	
	/// List of all Entities.
	EntityHolder entities;

	/// The camera of this scene.
	std::unique_ptr<Camera> camera;
};
}
