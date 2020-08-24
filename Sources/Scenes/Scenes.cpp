#include "Scenes.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
// TODO: Scenes should not require Graphics, this is because of Material and Mesh components.
const bool Scenes::Registered = Register(Stage::Normal, Requires<Graphics>());

Scenes::Scenes() {
}

void Scenes::Update() {
	if (!scene) return;

	if (!scene->started) {
		scene->Start();
		scene->started = true;
	}

	scene->Update();
}
}
