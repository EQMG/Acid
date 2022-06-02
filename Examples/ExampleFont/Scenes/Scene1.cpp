#include "Scene1.hpp"

#include <Uis/Uis.hpp>

namespace test {
Scene1::Scene1() :
	Scene(std::make_unique<Camera>()) {
	//pannable.SetTransform({UiMargins::All});
	Uis::Get()->GetCanvas().AddChild(&pannable);
}

void Scene1::Start() {
}

void Scene1::Update() {
	Scene::Update();
}

bool Scene1::IsPaused() const {
	return true;
}
}
