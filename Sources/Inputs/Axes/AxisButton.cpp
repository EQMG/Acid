#include "AxisButton.hpp"

namespace acid {
AxisButton::AxisButton(std::unique_ptr<Button> &&negative, std::unique_ptr<Button> &&positive) :
	negative(std::move(negative)),
	positive(std::move(positive)) {
	negative->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		onAxis(GetAmount());
	}, this);
	positive->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		onAxis(GetAmount());
	}, this);
}

float AxisButton::GetAmount() const {
	float amount = 0.0f;
	if (positive->IsDown())
		amount += 1.0f;
	if (negative->IsDown())
		amount -= 1.0f;

	return scale * amount;
}

Axis::ArgumentDescription AxisButton::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"negative", "button", "Button when pressed makes the amount read negative"},
		{"positive", "button", "Button when pressed makes the amount read positive"}
	};
}

void AxisButton::SetNegative(std::unique_ptr<Button> &&negative) {
	this->negative = std::move(negative);
	this->negative->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		onAxis(GetAmount());
	}, this);
}

void AxisButton::SetPositive(std::unique_ptr<Button> &&positive) {
	this->positive = std::move(positive);
	this->positive->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		onAxis(GetAmount());
	}, this);
}

const Node &operator>>(const Node &node, AxisButton &axisButton) {
	node["scale"].Get(axisButton.scale);
	node["negative"].Get(axisButton.negative);
	node["positive"].Get(axisButton.positive);
	return node;
}

Node &operator<<(Node &node, const AxisButton &axisButton) {
	node["scale"].Set(axisButton.scale);
	node["negative"].Set(axisButton.negative);
	node["positive"].Set(axisButton.positive);
	return node;
}
}
