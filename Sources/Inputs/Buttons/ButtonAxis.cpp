#include "ButtonAxis.hpp"

namespace acid {
ButtonAxis::ButtonAxis(std::unique_ptr<Axis> &&axis, float min, float max) :
	axis(std::move(axis)),
	min(min),
	max(max) {
	/*if (this->axis) {
		this->axis->OnAxis().Add([this](float value) {
			onButton(IsDown());
		}, this);
	}*/
}

bool ButtonAxis::IsDown() const {
	float amount = axis->GetAmount();
	return min < amount && amount < max;
}

Axis::ArgumentDescription ButtonAxis::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"axis", "axis", "The axis to sample"},
		{"min", "float", "Lower axis value bound"},
		{"max", "float", "Upper axis value bound"}
	};
}

void ButtonAxis::SetAxis(std::unique_ptr<Axis> &&axis) {
	this->axis = std::move(axis);
}

const Node &operator>>(const Node &node, ButtonAxis &buttonAxis) {
	node["inverted"].Get(buttonAxis.inverted);
	node["axis"].Get(buttonAxis.axis);
	node["min"].Get(buttonAxis.min);
	node["max"].Get(buttonAxis.max);
	return node;
}

Node &operator<<(Node &node, const ButtonAxis &buttonAxis) {
	node["inverted"].Set(buttonAxis.inverted);
	node["axis"].Set(buttonAxis.axis);
	node["min"].Set(buttonAxis.min);
	node["max"].Set(buttonAxis.max);
	return node;
}
}
