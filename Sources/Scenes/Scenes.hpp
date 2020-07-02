#pragma once

#include "Engine/Engine.hpp"
#include "Graphics/Graphics.hpp"
#include "Scene.hpp"
#include "SceneStructure.hpp"

namespace acid {
/**
 * @brief Module used for managing game scenes.
 */
class ACID_EXPORT Scenes : public Module::Registrar<Scenes> {
	inline static const bool Registered = Register(Stage::Normal, Requires<Graphics>());
public:
	Scenes();

	void Update() override;

	/**
	 * Gets the current scene.
	 * @return The current scene.
	 */
	Scene *GetScene() const { return scene.get(); }

	/**
	 * Sets the current scene to a new scene.
	 * @param scene The new scene.
	 */
	void SetScene(std::unique_ptr<Scene> &&scene) { this->scene = std::move(scene); }

	/**
	 * Gets the current camera object.
	 * @return The current camera.
	 */
	Camera *GetCamera() const { return scene ? scene->GetCamera() : nullptr; }

	/**
	 * Gets the scene physics system.
	 * @return The scenes physics system.
	 */
	ScenePhysics *GetPhysics() const { return scene ? scene->GetPhysics() : nullptr; }

	/**
	 * Gets the scene object structure.
	 * @return The scene object structure.
	 */
	SceneStructure *GetStructure() const { return scene ? scene->GetStructure() : nullptr; }

	/**
	 * Gets if the scene is paused.
	 * @return If the scene is paused.
	 */
	bool IsPaused() const { return scene ? scene->IsPaused() : false; }

private:
	std::unique_ptr<Scene> scene;
};
}
