#include "Panels.hpp"

namespace test {
Panels::Panels()  {
	m_hierarchy.SetTransform({{400, 500}, UiAnchor::LeftTop});
	AddChild(&m_hierarchy);

	m_inspector.SetTransform({{1.0f, 0.0f}, UiAnchor::RightTop, {0.3f, 1.0f}});
	AddChild(&m_inspector);
}

void Panels::UpdateObject() {
}
}
