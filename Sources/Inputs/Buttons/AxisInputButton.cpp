#include "AxisInputButton.hpp"

namespace acid {
AxisInputButton::AxisInputButton(std::unique_ptr<InputAxis> &&axis, float min, float max) :
	axis(std::move(axis)),
	min(min),
	max(max) {
	/*if (this->axis) {
		this->axis->OnAxis().Add([this](float value) {
			onButton(IsDown());
		}, this);
	}*/
}

bool AxisInputButton::IsDown() const {
	float amount = axis->GetAmount();
	return min < amount && amount < max;
}

InputAxis::ArgumentDescription AxisInputButton::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"axis", "axis", "The axis to sample"},
		{"min", "float", "Lower axis value bound"},
		{"max", "float", "Upper axis value bound"}
	};
}

void AxisInputButton::SetAxis(std::unique_ptr<InputAxis> &&axis) {
	this->axis = std::move(axis);
}

const Node &operator>>(const Node &node, AxisInputButton &inputButton) {
	node["inverted"].Get(inputButton.inverted);
	node["axis"].Get(inputButton.axis);
	node["min"].Get(inputButton.min);
	node["max"].Get(inputButton.max);
	return node;
}

Node &operator<<(Node &node, const AxisInputButton &inputButton) {
	node["inverted"].Set(inputButton.inverted);
	node["axis"].Set(inputButton.axis);
	node["min"].Set(inputButton.min);
	node["max"].Set(inputButton.max);
	return node;
}
}
