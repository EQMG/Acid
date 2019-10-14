#pragma once

#include "Engine/Engine.hpp"
#include "Scene.hpp"

namespace acid {
/**
 * @brief Module used for managing game scenes.
 */
class ACID_EXPORT Scenes : public Module::Registrar<Scenes> {
public:
	Scenes();

	void Update() override;

	/**
	 * Gets the current scene.
	 * @return The current scene.
	 */
	Scene *GetScene() const { return m_scene.get(); }

	/**
	 * Sets the current scene to a new scene.
	 * @param scene The new scene.
	 */
	void SetScene(std::unique_ptr<Scene> &&scene) { m_scene = std::move(scene); }

	/**
	 * Gets the current camera object.
	 * @return The current camera.
	 */
	Camera *GetCamera() const { return m_scene ? m_scene->GetCamera() : nullptr; }

	/**
	 * Gets if the scene is paused.
	 * @return If the scene is paused.
	 */
	bool IsPaused() const { return m_scene ? m_scene->IsPaused() : false; }

private:
	std::unique_ptr<Scene> m_scene;
};
}
