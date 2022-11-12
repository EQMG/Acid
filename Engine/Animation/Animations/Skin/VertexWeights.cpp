#include "VertexWeights.hpp"

#include "Utils/Enumerate.hpp"

namespace acid {
void VertexWeights::AddJointEffect(uint32_t jointId, float jointWeight) {
	for (auto [i, weight] : Enumerate(weights)) {
		if (jointWeight > weight) {
			jointIds[i] = jointId;
			weight = jointWeight;
			return;
		}
	}

	jointIds.emplace_back(jointId);
	weights.emplace_back(jointWeight);
}

void VertexWeights::LimitJointNumber(uint32_t max) {
	if (jointIds.size() > max) {
		std::vector<float> topWeights(max);
		auto total = SaveTopWeights(topWeights);
		RefillWeightList(topWeights, total);
		RemoveExcessJointIds(max);
	} else if (jointIds.size() < max) {
		FillEmptyWeights(max);
	}
}

void VertexWeights::FillEmptyWeights(uint32_t max) {
	while (jointIds.size() < max) {
		jointIds.emplace_back(0);
		weights.emplace_back(0.0f);
	}
}

float VertexWeights::SaveTopWeights(std::vector<float> &topWeightsArray) {
	float total = 0.0f;

	for (auto [i, topWeights] : Enumerate(topWeightsArray)) {
		topWeights = weights.at(i);
		total += topWeights;
	}

	return total;
}

void VertexWeights::RefillWeightList(const std::vector<float> &topWeights, float total) {
	weights.clear();

	for (const auto &topWeight : topWeights)
		weights.emplace_back(topWeight / total);
}

void VertexWeights::RemoveExcessJointIds(uint32_t max) {
	while (jointIds.size() > max)
		jointIds.pop_back();
}
}
