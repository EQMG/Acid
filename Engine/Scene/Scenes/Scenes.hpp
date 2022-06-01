#pragma once

#include "Engine/Engine.hpp"
#include "Scene.hpp"

namespace acid {
/**
 * @brief Module used for managing game scenes.
 */
class ACID_SCENE_EXPORT Scenes : public Module::Registrar<Scenes> {
	// TODO: Scenes should not require Graphics, this is because of Material and Mesh components.
	inline static const bool Registered = Register(Stage::Normal);
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
	
private:
	std::unique_ptr<Scene> scene;
};
}
