#include "ButtonInputAxis.hpp"

namespace acid {
ButtonInputAxis::ButtonInputAxis(std::unique_ptr<InputButton> &&negative, std::unique_ptr<InputButton> &&positive) :
	negative(std::move(negative)),
	positive(std::move(positive)) {
	negative->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		onAxis(GetAmount());
	});
	positive->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		onAxis(GetAmount());
	});
}

float ButtonInputAxis::GetAmount() const {
	float amount = 0.0f;
	if (positive->IsDown())
		amount += 1.0f;
	if (negative->IsDown())
		amount -= 1.0f;

	return scale * amount;
}

InputAxis::ArgumentDescription ButtonInputAxis::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"negative", "button", "InputButton when pressed makes the amount read negative"},
		{"positive", "button", "InputButton when pressed makes the amount read positive"}
	};
}

void ButtonInputAxis::SetNegative(std::unique_ptr<InputButton> &&negative) {
	this->negative = std::move(negative);
	this->negative->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		onAxis(GetAmount());
	});
}

void ButtonInputAxis::SetPositive(std::unique_ptr<InputButton> &&positive) {
	this->positive = std::move(positive);
	this->positive->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		onAxis(GetAmount());
	});
}

const Node &operator>>(const Node &node, ButtonInputAxis &inputAxis) {
	node["scale"].Get(inputAxis.scale);
	node["negative"].Get(inputAxis.negative);
	node["positive"].Get(inputAxis.positive);
	return node;
}

Node &operator<<(Node &node, const ButtonInputAxis &inputAxis) {
	node["scale"].Set(inputAxis.scale);
	node["negative"].Set(inputAxis.negative);
	node["positive"].Set(inputAxis.positive);
	return node;
}
}
