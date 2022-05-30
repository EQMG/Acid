#include "MouseInputAxis.hpp"

namespace acid {
MouseInputAxis::MouseInputAxis(uint8_t axis) :
	axis(axis) {
	Windows::Get()->GetWindow(0)->OnMousePosition().connect(this, [this](Vector2d value) {
		onAxis(GetAmount());
	});
}

float MouseInputAxis::GetAmount() const {
	return scale * static_cast<float>(Windows::Get()->GetWindow(0)->GetMousePositionDelta()[axis]) + offset;
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
