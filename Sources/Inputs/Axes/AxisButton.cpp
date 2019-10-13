#include "AxisButton.hpp"

namespace acid {
bool AxisButton::registered = Register("axisButton");

AxisButton::AxisButton(std::unique_ptr<Button> &&negative, std::unique_ptr<Button> &&positive) :
	m_negative(std::move(negative)),
	m_positive(std::move(positive)) {
	m_negative->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		m_onAxis(GetAmount());
	}, this);
	m_positive->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		m_onAxis(GetAmount());
	}, this);
}

float AxisButton::GetAmount() const {
	float amount = 0.0f;
	if (m_positive->IsDown())
		amount += 1.0f;
	if (m_negative->IsDown())
		amount -= 1.0f;

	return m_scale * amount;
}

Axis::ArgumentDescription AxisButton::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"negative", "button", "Button when pressed makes the amount read negative"},
		{"positive", "button", "Button when pressed makes the amount read positive"}
	};
}

void AxisButton::SetNegative(std::unique_ptr<Button> &&negative) {
	m_negative = std::move(negative);
	m_negative->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		m_onAxis(GetAmount());
	}, this);
}

void AxisButton::SetPositive(std::unique_ptr<Button> &&positive) {
	m_positive = std::move(positive);
	m_positive->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		m_onAxis(GetAmount());
	}, this);
}

const Node &operator>>(const Node &node, AxisButton &axisButton) {
	node["scale"].Get(axisButton.m_scale);
	node["negative"].Get(axisButton.m_negative);
	node["positive"].Get(axisButton.m_positive);
	return node;
}

Node &operator<<(Node &node, const AxisButton &axisButton) {
	node["scale"].Set(axisButton.m_scale);
	node["negative"].Set(axisButton.m_negative);
	node["positive"].Set(axisButton.m_positive);
	return node;
}
}
