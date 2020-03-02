#include "AxisMouse.hpp"

#include "Devices/Mouse.hpp"

namespace acid {
AxisMouse::AxisMouse(uint8_t axis) :
	axis(axis) {
	Mouse::Get()->OnPosition().Add([this](Vector2d value) {
		onAxis(GetAmount());
	}, this);
}

float AxisMouse::GetAmount() const {
	return scale * static_cast<float>(Mouse::Get()->GetPositionDelta()[axis]);
}

Axis::ArgumentDescription AxisMouse::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"axis", "axis", "The axis on the mouse delta is being checked"}
	};
}

const Node &operator>>(const Node &node, AxisMouse &axisMouse) {
	node["scale"].Get(axisMouse.scale);
	node["axis"].Get(axisMouse.axis);
	return node;
}

Node &operator<<(Node &node, const AxisMouse &axisMouse) {
	node["scale"].Set(axisMouse.scale);
	node["axis"].Set(axisMouse.axis);
	return node;
}
}
