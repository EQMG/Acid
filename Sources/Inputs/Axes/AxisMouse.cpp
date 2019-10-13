#include "AxisMouse.hpp"

#include "Devices/Mouse.hpp"

namespace acid {
bool AxisMouse::registered = Register("axisMouse");

AxisMouse::AxisMouse(uint8_t axis) :
	m_axis(axis) {
	Mouse::Get()->OnPosition().Add([this](Vector2d value) {
		m_onAxis(GetAmount());
	}, this);
}

float AxisMouse::GetAmount() const {
	return m_scale * static_cast<float>(Mouse::Get()->GetPositionDelta()[m_axis]);
}

Axis::ArgumentDescription AxisMouse::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"axis", "axis", "The axis on the mouse delta is being checked"}
	};
}

const Node &operator>>(const Node &node, AxisMouse &axisMouse) {
	node["scale"].Get(axisMouse.m_scale);
	node["axis"].Get(axisMouse.m_axis);
	return node;
}

Node &operator<<(Node &node, const AxisMouse &axisMouse) {
	node["scale"].Set(axisMouse.m_scale);
	node["axis"].Set(axisMouse.m_axis);
	return node;
}
}
