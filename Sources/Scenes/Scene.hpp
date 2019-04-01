#pragma once

#include "Camera.hpp"
#include "ScenePhysics.hpp"
#include "SceneStructure.hpp"

namespace acid
{
/**
 * @brief Class that is used to represent a scene.
 */
class ACID_EXPORT Scene
{
public:
	/**
	 * Creates a new scene.
	 * @param camera The scenes camera.
	 */
	explicit Scene(Camera *camera) :
		m_camera(camera),
		m_structure(std::make_unique<SceneStructure>()),
		m_physics(std::make_unique<ScenePhysics>()),
		m_started(false)
	{
	}

	virtual ~Scene() = default;

	virtual void Start() = 0;

	/**
	 * Run when updating the scene.
	 */
	virtual void Update() = 0;

	/**
	 * Gets the current camera object.
	 * @return The current camera.
	 */
	Camera *GetCamera() const { return m_camera.get(); }

	/**
	 * Sets the current camera to a new camera.
	 * @param camera The new camera.
	 */
	void SetCamera(Camera *camera) { m_camera.reset(camera); }

	/**
	 * Gets the scene object structure.
	 * @return The scene object structure.
	 */
	SceneStructure *GetStructure() const { return m_structure.get(); }

	/**
	 * Gets the scene physics system.
	 * @return The scenes physics system.
	 */
	ScenePhysics *GetPhysics() const { return m_physics.get(); }

	/**
	 * Gets if the scene is paused.
	 * @return If the scene is paused.
	 */
	virtual bool IsPaused() const = 0;

private:
	friend class Scenes;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<SceneStructure> m_structure;
	std::unique_ptr<ScenePhysics> m_physics;
	bool m_started;
};
}
