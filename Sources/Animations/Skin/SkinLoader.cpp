#include "SkinLoader.hpp"

namespace acid {
SkinLoader::SkinLoader(NodeReturn libraryControllers, uint32_t maxWeights) :
	m_skinData(libraryControllers["controller"]["skin"]),
	m_maxWeights(maxWeights) {
	LoadJointsList();
	auto weights = LoadWeights();
	auto weightsDataNode = m_skinData["vertex_weights"];
	auto effectorJointCounts = GetEffectiveJointsCounts(weightsDataNode);
	GetSkinWeights(weightsDataNode, effectorJointCounts, weights);
}

void SkinLoader::LoadJointsList() {
	auto inputNode = m_skinData["vertex_weights"];
	auto jointDataIdNode = inputNode["input"]->GetPropertyWithValue("-semantic", "JOINT")["-source"]->GetValue().substr(1);
	auto jointDataId = inputNode["input"]->GetPropertyWithValue("-semantic", "JOINT")["-source"]->GetValue().substr(1);
	auto jointsNode = m_skinData["source"]->GetPropertyWithValue("-id", jointDataId)["Name_array"];
	m_jointOrder = String::Split(jointsNode["#text"]->GetValue(), ' ');
}

std::vector<float> SkinLoader::LoadWeights() const {
	auto inputNode = m_skinData["vertex_weights"];
	auto weightsDataId = inputNode["input"]->GetPropertyWithValue("-semantic", "WEIGHT")["-source"]->GetValue().substr(1);
	auto weightsNode = m_skinData["source"]->GetPropertyWithValue("-id", weightsDataId)["float_array"]["#text"];

	auto rawDatas = String::Split(weightsNode->GetValue(), ' ');
	std::vector<float> weights(rawDatas.size());

	for (uint32_t i = 0; i < weights.size(); i++) {
		weights[i] = String::From<float>(rawDatas[i]);
	}

	return weights;
}

std::vector<uint32_t> SkinLoader::GetEffectiveJointsCounts(const Node &weightsDataNode) const {
	auto rawData = String::Split(weightsDataNode["vcount"]->GetValue(), ' ');
	std::vector<uint32_t> counts(rawData.size());

	for (uint32_t i = 0; i < rawData.size(); i++) {
		counts[i] = String::From<uint32_t>(rawData[i]);
	}

	return counts;
}

void SkinLoader::GetSkinWeights(const Node &weightsDataNode, const std::vector<uint32_t> &counts, const std::vector<float> &weights) {
	auto rawData = String::Split(weightsDataNode["v"]->GetValue(), ' ');
	uint32_t pointer = 0;

	for (auto count : counts) {
		VertexWeights skinData;

		for (uint32_t i = 0; i < count; i++) {
			auto jointId = String::From<uint32_t>(rawData[pointer++]);
			auto weightId = String::From<uint32_t>(rawData[pointer++]);
			skinData.AddJointEffect(jointId, weights[weightId]);
		}

		skinData.LimitJointNumber(m_maxWeights);
		m_vertexWeights.emplace_back(skinData);
	}
}
}
