#pragma once

#include "Camera.hpp"
#include "ScenePhysics.hpp"
#include "SceneStructure.hpp"

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
	explicit Scene(std::unique_ptr<Camera> &&camera) :
		camera(std::move(camera)),
		structure(std::make_unique<SceneStructure>()),
		physics(std::make_unique<ScenePhysics>()) {
	}

	virtual ~Scene() = default;

	/**
	 * Run when switching to this scene from another.
	 */
	virtual void Start() = 0;

	/**
	 * Run when updating the scene.
	 */
	virtual void Update() = 0;

	/**
	 * Gets the scene physics system.
	 * @return The scenes physics system.
	 */
	ScenePhysics *GetPhysics() const { return physics.get(); }

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
	 * Gets the scene object structure.
	 * @return The scene object structure.
	 */
	SceneStructure *GetStructure() const { return structure.get(); }

	/**
	 * Gets if the scene is paused.
	 * @return If the scene is paused.
	 */
	virtual bool IsPaused() const = 0;

private:
	bool started = false;
	std::unique_ptr<ScenePhysics> physics;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<SceneStructure> structure;
};
}
