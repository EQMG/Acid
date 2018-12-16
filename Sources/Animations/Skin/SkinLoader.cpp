#include "SkinLoader.hpp"

namespace acid
{
	SkinLoader::SkinLoader(Metadata *libraryControllers, const uint32_t &maxWeights) :
		m_skinData(libraryControllers->FindChild("controller")->FindChild("skin")),
		m_maxWeights(maxWeights),
		m_jointOrder(std::vector<std::string>()),
		m_vertexWeights(std::vector<VertexWeights>())
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
		std::string jointDataId = inputNode->FindChildWithAttribute("input", "semantic", "JOINT")->FindAttribute("source").substr(1);
		auto jointsNode = m_skinData->FindChildWithAttribute("source", "id", jointDataId)->FindChild("Name_array");
		m_jointOrder = String::Split(jointsNode->GetValue(), " ");
	}

	std::vector<float> SkinLoader::LoadWeights()
	{
		auto inputNode = m_skinData->FindChild("vertex_weights");
		std::string weightsDataId = inputNode->FindChildWithAttribute("input", "semantic", "WEIGHT")->FindAttribute("source").substr(1);
		auto weightsNode = m_skinData->FindChildWithAttribute("source", "id", weightsDataId)->FindChild("float_array");

		auto rawData = String::Split(weightsNode->GetValue(), " ");
		auto weights = std::vector<float>(rawData.size());

		for (uint32_t i = 0; i < weights.size(); i++)
		{
			weights[i] = String::From<float>(rawData[i]);
		}

		return weights;
	}

	std::vector<uint32_t> SkinLoader::GetEffectiveJointsCounts(Metadata *weightsDataNode)
	{
		auto rawData = String::Split(weightsDataNode->FindChild("vcount")->GetString(), " ");
		auto counts = std::vector<uint32_t>(rawData.size());

		for (uint32_t i = 0; i < rawData.size(); i++)
		{
			counts[i] = String::From<uint32_t>(rawData[i]);
		}

		return counts;
	}

	void SkinLoader::GetSkinWeights(Metadata *weightsDataNode, const std::vector<uint32_t> &counts, const std::vector<float> &weights)
	{
		auto rawData = String::Split(weightsDataNode->FindChild("v")->GetString(), " ");
		uint32_t pointer = 0;

		for (auto count : counts)
		{
			auto skinData = VertexWeights();

			for (uint32_t i = 0; i < count; i++)
			{
				uint32_t jointId = String::From<uint32_t>(rawData[pointer++]);
				uint32_t weightId = String::From<uint32_t>(rawData[pointer++]);
				skinData.AddJointEffect(jointId, weights[weightId]);
			}

			skinData.LimitJointNumber(m_maxWeights);
			m_vertexWeights.emplace_back(skinData);
		}
	}
}
