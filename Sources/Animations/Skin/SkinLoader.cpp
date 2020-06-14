#include "SkinLoader.hpp"

#include "Utils/Enumerate.hpp"

namespace acid {
SkinLoader::SkinLoader(NodeConstView &&libraryControllers, uint32_t maxWeights) :
	skinData(libraryControllers["controller"]["skin"]),
	maxWeights(maxWeights) {
	LoadJointsList();
	auto weights = LoadWeights();
	auto weightsDataNode = skinData["vertex_weights"];
	auto effectorJointCounts = GetEffectiveJointsCounts(weightsDataNode);
	GetSkinWeights(weightsDataNode, effectorJointCounts, weights);
}

void SkinLoader::LoadJointsList() {
	auto inputNode = skinData["vertex_weights"];
	auto jointDataIdNode = inputNode["input"].GetPropertyWithValue("-semantic", "JOINT")["-source"].Get<std::string>().substr(1);
	auto jointDataId = inputNode["input"].GetPropertyWithValue("-semantic", "JOINT")["-source"].Get<std::string>().substr(1);
	auto jointsNode = skinData["source"].GetPropertyWithValue("-id", jointDataId)["Name_array"];
	jointOrder = String::Split(jointsNode.Get<std::string>(), ' ');
}

std::vector<float> SkinLoader::LoadWeights() const {
	auto inputNode = skinData["vertex_weights"];
	auto weightsDataId = inputNode["input"].GetPropertyWithValue("-semantic", "WEIGHT")["-source"].Get<std::string>().substr(1);
	auto weightsNode = skinData["source"].GetPropertyWithValue("-id", weightsDataId)["float_array"];

	auto rawDatas = String::Split(weightsNode.Get<std::string>(), ' ');
	std::vector<float> weights(rawDatas.size());

	for (auto &&[i, weight] : Enumerate(weights))
		weight = String::From<float>(rawDatas[i]);

	return weights;
}

std::vector<uint32_t> SkinLoader::GetEffectiveJointsCounts(const Node &weightsDataNode) const {
	auto rawData = String::Split(weightsDataNode["vcount"].Get<std::string>(), ' ');
	std::vector<uint32_t> counts(rawData.size());

	for (auto &&[i, count] : Enumerate(counts))
		count = String::From<uint32_t>(rawData[i]);

	return counts;
}

void SkinLoader::GetSkinWeights(const Node &weightsDataNode, const std::vector<uint32_t> &counts, const std::vector<float> &weights) {
	auto rawData = String::Split(weightsDataNode["v"].Get<std::string>(), ' ');
	uint32_t pointer = 0;

	for (auto count : counts) {
		VertexWeights skinData;

		for (uint32_t i = 0; i < count; i++) {
			auto jointId = String::From<uint32_t>(rawData[pointer++]);
			auto weightId = String::From<uint32_t>(rawData[pointer++]);
			skinData.AddJointEffect(jointId, weights[weightId]);
		}

		skinData.LimitJointNumber(maxWeights);
		vertexWeights.emplace_back(skinData);
	}
}
}
