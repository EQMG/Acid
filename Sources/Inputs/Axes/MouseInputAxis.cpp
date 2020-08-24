#include "MouseInputAxis.hpp"

#include "Devices/Mouse.hpp"

namespace acid {
const bool MouseInputAxis::Registered = Register("mouse");

MouseInputAxis::MouseInputAxis(uint8_t axis) :
	axis(axis) {
	Mouse::Get()->OnPosition().connect(this, [this](Vector2d value) {
		onAxis(GetAmount());
	});
}

float MouseInputAxis::GetAmount() const {
	return scale * static_cast<float>(Mouse::Get()->GetPositionDelta()[axis]);
}

InputAxis::ArgumentDescription MouseInputAxis::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"axis", "axis", "The axis on the mouse delta is being checked"}
	};
}

const Node &operator>>(const Node &node, MouseInputAxis &inputAxis) {
	node["scale"].Get(inputAxis.scale);
	node["axis"].Get(inputAxis.axis);
	return node;
}

Node &operator<<(Node &node, const MouseInputAxis &inputAxis) {
	node["scale"].Set(inputAxis.scale);
	node["axis"].Set(inputAxis.axis);
	return node;
}
}
