#include "SkinLoader.hpp"

namespace fl
{
	SkinLoader::SkinLoader(LoadedValue *libraryControllers, const int &maxWeights) :
		m_skinData(libraryControllers->GetChild("controller")->GetChild("skin")),
		m_maxWeights(maxWeights),
		m_jointOrder(std::vector<std::string>()),
		m_verticesSkinData(std::vector<VertexSkinData *>())
	{
		LoadJointsList();
		auto weights = LoadWeights();
		LoadedValue *weightsDataNode = m_skinData->GetChild("vertex_weights");
		auto effectorJointCounts = GetEffectiveJointsCounts(weightsDataNode);
		GetSkinData(weightsDataNode, effectorJointCounts, weights);
	}

	SkinLoader::~SkinLoader()
	{
		for (auto vertex : m_verticesSkinData)
		{
			delete vertex;
		}
	}

	void SkinLoader::LoadJointsList()
	{
		LoadedValue *inputNode = m_skinData->GetChild("vertex_weights");
		std::string jointDataId = inputNode->GetChildWithAttribute("input", "-semantic", "JOINT")->GetChild("-source")->GetString().substr(1);
		LoadedValue *jointsNode = m_skinData->GetChildWithAttribute("source", "-id", jointDataId)->GetChild("Name_array");
		m_jointOrder = FormatString::Split(jointsNode->GetChild("#text")->GetString(), " ");
	}

	std::vector<float> SkinLoader::LoadWeights()
	{
		LoadedValue *inputNode = m_skinData->GetChild("vertex_weights");
		std::string weightsDataId = inputNode->GetChildWithAttribute("input", "-semantic", "WEIGHT")->GetChild("-source")->GetString().substr(1);
		LoadedValue *weightsNode = m_skinData->GetChildWithAttribute("source", "-id", weightsDataId)->GetChild("float_array");

		auto rawData = FormatString::Split(weightsNode->GetChild("#text")->GetString(), " ");
		std::vector<float> weights = std::vector<float>(rawData.size());

		for (int i = 0; i < weights.size(); i++)
		{
			weights[i] = std::stof(rawData[i]);
		}

		return weights;
	}

	std::vector<int> SkinLoader::GetEffectiveJointsCounts(LoadedValue *weightsDataNode)
	{
		auto rawData = FormatString::Split(weightsDataNode->GetChild("vcount")->GetString(), " ");
		std::vector<int> counts = std::vector<int>(rawData.size());

		for (int i = 0; i < rawData.size(); i++)
		{
			counts[i] = std::stoi(rawData[i]);
		}

		return counts;
	}

	void SkinLoader::GetSkinData(LoadedValue *weightsDataNode, const std::vector<int> &counts, const std::vector<float> &weights)
	{
		auto rawData = FormatString::Split(weightsDataNode->GetChild("v")->GetString(), " ");
		int pointer = 0;

		for (int count : counts)
		{
			auto skinData = new VertexSkinData();

			for (int i = 0; i < count; i++)
			{
				int jointId = std::stoi(rawData[pointer++]);
				int weightId = std::stoi(rawData[pointer++]);
				skinData->AddJointEffect(jointId, weights[weightId]);
			}

			skinData->LimitJointNumber(m_maxWeights);
			m_verticesSkinData.push_back(skinData);
		}
	}
}
