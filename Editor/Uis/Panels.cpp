#include "Panels.hpp"

namespace test {
Panels::Panels()  {
	//hierarchy.SetTransform({{400, 500}, UiAnchor::LeftTop});
	AddChild(&hierarchy);

	//inspector.SetTransform({{1.0f, 0.0f}, UiAnchor::RightTop, {0.3f, 1.0f}});
	AddChild(&inspector);
}

void Panels::UpdateObject() {
}
}
