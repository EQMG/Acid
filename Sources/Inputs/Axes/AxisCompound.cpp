#include "AxisCompound.hpp"

namespace acid {
AxisCompound::AxisCompound(std::vector<std::unique_ptr<Axis>> &&axes) :
	axes(std::move(axes)) {
	ConnectAxes();
}

float AxisCompound::GetAmount() const {
	float result = 0.0f;
	for (const auto &axis : axes)
		result += axis->GetAmount();

	return scale * std::clamp(result, -1.0f, 1.0f);
}

Axis::ArgumentDescription AxisCompound::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"axes", "axis[]", "The axes that will be combined into a compound axis"}
	};
}

Axis *AxisCompound::AddAxis(std::unique_ptr<Axis> &&axis) {
	auto &result = axes.emplace_back(std::move(axis));
	ConnectAxis(result);
	return result.get();
}

void AxisCompound::RemoveAxis(Axis *axis) {
	//axis->OnAxis().RemoveObservers(this);
	axes.erase(std::remove_if(axes.begin(), axes.end(), [axis](std::unique_ptr<Axis> &a) {
		return a.get() == axis;
	}), axes.end());
}

void AxisCompound::ConnectAxis(std::unique_ptr<Axis> &axis) {
	axis->OnAxis().Add([this](float value) {
		onAxis(GetAmount());
	}, this);
}

void AxisCompound::ConnectAxes() {
	for (auto &axis : axes)
		ConnectAxis(axis);
}

const Node &operator>>(const Node &node, AxisCompound &axisCompound) {
	node["scale"].Get(axisCompound.scale);
	node["axes"].Get(axisCompound.axes);
	axisCompound.ConnectAxes();
	return node;
}

Node &operator<<(Node &node, const AxisCompound &axisCompound) {
	node["scale"].Set(axisCompound.scale);
	node["axes"].Set(axisCompound.axes);
	return node;
}
}
