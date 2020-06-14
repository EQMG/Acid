#include "Shadows.hpp"

#include "Scenes/Scenes.hpp"

namespace acid {
Shadows::Shadows() :
	lightDirection(0.5f, 0.0f, 0.5f),
	shadowSize(4096),
	shadowPcf(1),
	shadowBias(0.001f),
	shadowDarkness(0.6f),
	shadowTransition(11.0f),
	shadowBoxOffset(9.0f),
	shadowBoxDistance(70.0f) {
}

void Shadows::Update() {
	if (auto camera = Scenes::Get()->GetCamera())
		shadowBox.Update(*camera, lightDirection, shadowBoxOffset, shadowBoxDistance);
}
}
