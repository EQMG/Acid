#include "SkinLoader.hpp"

namespace acid
{
SkinLoader::SkinLoader(const Node *libraryControllers, uint32_t maxWeights) :
	m_skinData{libraryControllers->FindChild("controller")->FindChild("skin")},
	m_maxWeights{maxWeights}
{
	LoadJointsList();
	auto weights = LoadWeights();
	auto weightsDataNode = m_skinData->FindChild("vertex_weights");
	auto effectorJointCounts = GetEffectiveJointsCounts(weightsDataNode);
	GetSkinWeights(weightsDataNode, effectorJointCounts, weights);
}

void SkinLoader::LoadJointsList()
{
	auto inputNode = m_skinData->FindChild("vertex_weights");
	auto jointDataId = inputNode->FindChildWithAttribute("input", "semantic", "JOINT")->FindAttribute("source")->substr(1);
	auto jointsNode = m_skinData->FindChildWithAttribute("source", "id", jointDataId)->FindChild("Name_array");
	m_jointOrder = String::Split(jointsNode->GetValue(), ' ');
}

std::vector<float> SkinLoader::LoadWeights() const
{
	auto inputNode = m_skinData->FindChild("vertex_weights");
	auto weightsDataId = inputNode->FindChildWithAttribute("input", "semantic", "WEIGHT")->FindAttribute("source")->substr(1);
	auto weightsNode = m_skinData->FindChildWithAttribute("source", "id", weightsDataId)->FindChild("float_array");

	auto rawDatas = String::Split(weightsNode->GetValue(), ' ');
	std::vector<float> weights(rawDatas.size());

	for (uint32_t i{}; i < weights.size(); i++)
	{
		weights[i] = String::From<float>(rawDatas[i]);
	}

	return weights;
}

std::vector<uint32_t> SkinLoader::GetEffectiveJointsCounts(const Node *weightsDataNode) const
{
	auto rawData = String::Split(weightsDataNode->FindChild("vcount")->GetValue(), ' ');
	std::vector<uint32_t> counts(rawData.size());

	for (uint32_t i{}; i < rawData.size(); i++)
	{
		counts[i] = String::From<uint32_t>(rawData[i]);
	}

	return counts;
}

void SkinLoader::GetSkinWeights(const Node *weightsDataNode, const std::vector<uint32_t> &counts, const std::vector<float> &weights)
{
	auto rawData = String::Split(weightsDataNode->FindChild("v")->GetValue(), ' ');
	uint32_t pointer{};

	for (auto count : counts)
	{
		VertexWeights skinData;

		for (uint32_t i{}; i < count; i++)
		{
			auto jointId = String::From<uint32_t>(rawData[pointer++]);
			auto weightId = String::From<uint32_t>(rawData[pointer++]);
			skinData.AddJointEffect(jointId, weights[weightId]);
		}

		skinData.LimitJointNumber(m_maxWeights);
		m_vertexWeights.emplace_back(skinData);
	}
}
}
