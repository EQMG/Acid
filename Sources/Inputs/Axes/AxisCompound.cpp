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

Axis *AxisCompound::AddAxis(std::unique_ptr<Axis> &&axis) {
	auto &result = m_axes.emplace_back(std::move(axis));
	ConnectAxis(result);
	return result.get();
}

void AxisCompound::RemoveAxis(Axis *axis) {
	//axis->OnAxis().RemoveObservers(this);
	m_axes.erase(std::remove_if(m_axes.begin(), m_axes.end(), [axis](std::unique_ptr<Axis> &a) {
		return a.get() == axis;
	}), m_axes.end());
}

void AxisCompound::ConnectAxis(std::unique_ptr<Axis> &axis) {
	axis->OnAxis().Add([this](float value) {
		m_onAxis(GetAmount());
	}, this);
}

void AxisCompound::ConnectAxes() {
	for (auto &axis : m_axes)
		ConnectAxis(axis);
}

const Node &operator>>(const Node &node, AxisCompound &axisCompound) {
	node["scale"].Get(axisCompound.m_scale);
	node["axes"].Get(axisCompound.m_axes);
	axisCompound.ConnectAxes();
	return node;
}

Node &operator<<(Node &node, const AxisCompound &axisCompound) {
	node["scale"].Set(axisCompound.m_scale);
	node["axes"].Set(axisCompound.m_axes);
	return node;
}
}
