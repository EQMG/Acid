#include "ButtonAxis.hpp"

namespace acid {
bool ButtonAxis::registered = Register("buttonAxis");

ButtonAxis::ButtonAxis(std::unique_ptr<Axis> &&axis, float min, float max) :
	m_axis(std::move(axis)),
	m_min(min),
	m_max(max) {
	/*if (m_axis) {
		m_axis->OnAxis().Add([this](float value) {
			m_onButton(IsDown());
		}, this);
	}*/
}

bool ButtonAxis::IsDown() const {
	float amount = m_axis->GetAmount();
	return m_min < amount && amount < m_max;
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
	m_axis = std::move(axis);
}

const Node &operator>>(const Node &node, ButtonAxis &buttonAxis) {
	node["inverted"].Get(buttonAxis.m_inverted);
	node["axis"].Get(buttonAxis.m_axis);
	node["min"].Get(buttonAxis.m_min);
	node["max"].Get(buttonAxis.m_max);
	return node;
}

Node &operator<<(Node &node, const ButtonAxis &buttonAxis) {
	node["inverted"].Set(buttonAxis.m_inverted);
	node["axis"].Set(buttonAxis.m_axis);
	node["min"].Set(buttonAxis.m_min);
	node["max"].Set(buttonAxis.m_max);
	return node;
}
}
