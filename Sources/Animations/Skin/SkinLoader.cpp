#include "SkinLoader.hpp"

namespace acid
{
	SkinLoader::SkinLoader(const std::shared_ptr<LoadedValue> &libraryControllers, const int &maxWeights) :
		m_skinData(libraryControllers->FindChild("controller")->FindChild("skin")),
		m_maxWeights(maxWeights),
		m_jointOrder(std::vector<std::string>()),
		m_verticesSkinData(std::vector<VertexSkinData *>())
	{
		LoadJointsList();
		auto weights = LoadWeights();
		auto weightsDataNode = m_skinData->FindChild("vertex_weights");
		auto effectorJointCounts = GetEffectiveJointsCounts(weightsDataNode);
		GetSkinData(weightsDataNode, effectorJointCounts, weights);
	}

	SkinLoader::~SkinLoader()
	{
		for (auto &vertex : m_verticesSkinData)
		{
			delete vertex;
		}
	}

	void SkinLoader::LoadJointsList()
	{
		auto inputNode = m_skinData->FindChild("vertex_weights");
		std::string jointDataId = inputNode->FindChildWithAttribute("input", "semantic", "JOINT")->FindAttribute(
			"source").substr(1);
		auto jointsNode = m_skinData->FindChildWithAttribute("source", "id", jointDataId)->FindChild("Name_array");
		m_jointOrder = FormatString::Split(jointsNode->GetValue(), " ");
	}

	std::vector<float> SkinLoader::LoadWeights()
	{
		auto inputNode = m_skinData->FindChild("vertex_weights");
		std::string weightsDataId = inputNode->FindChildWithAttribute("input", "semantic", "WEIGHT")->FindAttribute(
			"source").substr(1);
		auto weightsNode = m_skinData->FindChildWithAttribute("source", "id", weightsDataId)->FindChild("float_array");

		auto rawData = FormatString::Split(weightsNode->GetValue(), " ");
		auto weights = std::vector<float>(rawData.size());

		for (uint32_t i = 0; i < weights.size(); i++)
		{
			weights[i] = std::stof(rawData[i]);
		}

		return weights;
	}

	std::vector<int> SkinLoader::GetEffectiveJointsCounts(std::shared_ptr<LoadedValue> &weightsDataNode)
	{
		auto rawData = FormatString::Split(weightsDataNode->FindChild("vcount")->GetString(), " ");
		auto counts = std::vector<int>(rawData.size());

		for (uint32_t i = 0; i < rawData.size(); i++)
		{
			counts[i] = std::stoi(rawData[i]);
		}

		return counts;
	}

	void SkinLoader::GetSkinData(std::shared_ptr<LoadedValue> &weightsDataNode, const std::vector<int> &counts, const std::vector<float> &weights)
	{
		auto rawData = FormatString::Split(weightsDataNode->FindChild("v")->GetString(), " ");
		int pointer = 0;

		for (auto count : counts)
		{
			auto skinData = new VertexSkinData();

			for (int i = 0; i < count; i++)
			{
				int jointId = std::stoi(rawData[pointer++]);
				int weightId = std::stoi(rawData[pointer++]);
				skinData->AddJointEffect(jointId, weights[weightId]);
			}

			skinData->LimitJointNumber(m_maxWeights);
			m_verticesSkinData.emplace_back(skinData);
		}
	}
}
