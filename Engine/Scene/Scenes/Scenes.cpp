#include "Scenes.hpp"

namespace acid {
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
