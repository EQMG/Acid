#include "VertexWeights.hpp"

#include "Helpers/Enumerate.hpp"

namespace acid {
void VertexWeights::AddJointEffect(uint32_t jointId, float jointWeight) {
	for (auto &&[i, weight] : Enumerate(m_weights)) {
		if (jointWeight > weight) {
			m_jointIds[i] = jointId;
			weight = jointWeight;
			return;
		}
	}

	m_jointIds.emplace_back(jointId);
	m_weights.emplace_back(jointWeight);
}

void VertexWeights::LimitJointNumber(uint32_t max) {
	if (m_jointIds.size() > max) {
		std::vector<float> topWeights(max);
		auto total = SaveTopWeights(topWeights);
		RefillWeightList(topWeights, total);
		RemoveExcessJointIds(max);
	} else if (m_jointIds.size() < max) {
		FillEmptyWeights(max);
	}
}

void VertexWeights::FillEmptyWeights(uint32_t max) {
	while (m_jointIds.size() < max) {
		m_jointIds.emplace_back(0);
		m_weights.emplace_back(0.0f);
	}
}

float VertexWeights::SaveTopWeights(std::vector<float> &topWeightsArray) {
	float total = 0.0f;

	for (auto &&[i, topWeights] : Enumerate(topWeightsArray)) {
		topWeights = m_weights.at(i);
		total += topWeights;
	}

	return total;
}

void VertexWeights::RefillWeightList(const std::vector<float> &topWeights, float total) {
	m_weights.clear();

	for (const auto &topWeight : topWeights) {
		m_weights.emplace_back(topWeight / total);
	}
}

void VertexWeights::RemoveExcessJointIds(uint32_t max) {
	while (m_jointIds.size() > max) {
		m_jointIds.pop_back();
	}
}
}
