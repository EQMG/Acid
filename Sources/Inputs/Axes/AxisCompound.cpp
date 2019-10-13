#include "AxisCompound.hpp"

namespace acid {
bool AxisCompound::registered = Register("axisCompound");

AxisCompound::AxisCompound(std::vector<std::unique_ptr<Axis>> &&axes) :
	m_axes(std::move(axes)) {
	ConnectAxes();
}

float AxisCompound::GetAmount() const {
	float result = 0.0f;
	for (const auto &axis : m_axes)
		result += axis->GetAmount();

	return m_scale * std::clamp(result, -1.0f, 1.0f);
}

Axis::ArgumentDescription AxisCompound::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"axes", "axis[]", "The axes that will be combined into a compound axis"}
	};
}

void AxisCompound::ConnectAxes() {
	for (auto &axis : m_axes) {
		axis->OnAxis().Add([this](float value) {
			m_onAxis(GetAmount());
		}, this);
	}
}

const Node &operator>>(const Node &node, AxisCompound &axisCompound) {
	node["scale"].Get(axisCompound.m_scale);
	node["axes"].Get(axisCompound.m_axes);
	return node;
}

Node &operator<<(Node &node, const AxisCompound &axisCompound) {
	node["scale"].Set(axisCompound.m_scale);
	node["axes"].Set(axisCompound.m_axes);
	return node;
}
}
