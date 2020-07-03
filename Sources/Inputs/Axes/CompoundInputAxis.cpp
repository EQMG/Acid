#include "CompoundInputAxis.hpp"

namespace acid {
CompoundInputAxis::CompoundInputAxis(std::vector<std::unique_ptr<InputAxis>> &&axes) :
	axes(std::move(axes)) {
	ConnectAxes();
}

float CompoundInputAxis::GetAmount() const {
	float result = 0.0f;
	for (const auto &axis : axes)
		result += axis->GetAmount();

	return scale * std::clamp(result, -1.0f, 1.0f);
}

InputAxis::ArgumentDescription CompoundInputAxis::GetArgumentDescription() const {
	return {
		{"scale", "float", "Output amount scalar"},
		{"axes", "axis[]", "The axes that will be combined into a compound axis"}
	};
}

InputAxis *CompoundInputAxis::AddAxis(std::unique_ptr<InputAxis> &&axis) {
	auto &result = axes.emplace_back(std::move(axis));
	ConnectAxis(result);
	return result.get();
}

void CompoundInputAxis::RemoveAxis(InputAxis *axis) {
	//axis->OnAxis().RemoveObservers(this);
	axes.erase(std::remove_if(axes.begin(), axes.end(), [axis](const auto &a) {
		return a.get() == axis;
	}), axes.end());
}

void CompoundInputAxis::ConnectAxis(std::unique_ptr<InputAxis> &axis) {
	axis->OnAxis().connect(this, [this](float value) {
		onAxis(GetAmount());
	});
}

void CompoundInputAxis::ConnectAxes() {
	for (auto &axis : axes)
		ConnectAxis(axis);
}

const Node &operator>>(const Node &node, CompoundInputAxis &inputAxis) {
	node["scale"].Get(inputAxis.scale);
	node["axes"].Get(inputAxis.axes);
	inputAxis.ConnectAxes();
	return node;
}

Node &operator<<(Node &node, const CompoundInputAxis &inputAxis) {
	node["scale"].Set(inputAxis.scale);
	node["axes"].Set(inputAxis.axes);
	return node;
}
}
