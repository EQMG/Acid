#include "Scenes.hpp"

namespace acid {
Scenes::Scenes() {
}

void Scenes::Update() {
	if (!scene)
		return;

	if (!scene->started) {
		scene->Start();
		scene->started = true;
	}

	scene->Update();
	scene->GetPhysics()->Update();
	if (scene->GetStructure())
		scene->GetStructure()->Update();
	if (scene->GetCamera())
		scene->GetCamera()->Update();
}
}
