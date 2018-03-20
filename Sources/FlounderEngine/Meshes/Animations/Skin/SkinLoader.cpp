#include "SkinLoader.hpp"

namespace Flounder
{
	SkinLoader::SkinLoader(LoadedValue *libraryControllers, const int &maxWeights) :
		m_skinData(libraryControllers->GetChild("controller")->GetChild("skin")),
		m_maxWeights(maxWeights),
		m_skinningData(nullptr)
	{
		auto jointsList = LoadJointsList();
		auto weights = LoadWeights();
		LoadedValue *weightsDataNode = m_skinData->GetChild("vertex_weights");
		auto effectorJointCounts = GetEffectiveJointsCounts(weightsDataNode);
		auto vertexWeights = GetSkinData(weightsDataNode, effectorJointCounts, weights);
		m_skinningData = new SkinningData(jointsList, vertexWeights);
	}

	SkinLoader::~SkinLoader()
	{
		delete m_skinningData;
	}

	std::vector<std::string> SkinLoader::LoadJointsList()
	{
		LoadedValue *inputNode = m_skinData->GetChild("vertex_weights");
		std::string jointDataId = inputNode->GetChildWithAttribute("input", "-semantic", "JOINT")->GetChild("-source")->GetString().substr(1);
		LoadedValue *jointsNode = m_skinData->GetChildWithAttribute("source", "-id", jointDataId)->GetChild("Name_array");
		return FormatString::Split(jointsNode->GetChild("#text")->GetString(), " ");
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
			weights[i] = stof(rawData[i]);
		}

		return weights;
	}

	std::vector<int> SkinLoader::GetEffectiveJointsCounts(LoadedValue *weightsDataNode)
	{
		auto rawData = FormatString::Split(weightsDataNode->GetChild("vcount")->GetString(), " ");
		std::vector<int> counts = std::vector<int>(rawData.size());

		for (int i = 0; i < rawData.size(); i++)
		{
			counts[i] = stoi(rawData[i]);
		}

		return counts;
	}

	std::vector<VertexSkinData *> SkinLoader::GetSkinData(LoadedValue *weightsDataNode, const std::vector<int> &counts, const std::vector<float> &weights)
	{
		auto rawData = FormatString::Split(weightsDataNode->GetChild("v")->GetString(), " ");
		std::vector<VertexSkinData *> skinningData = std::vector<VertexSkinData *>();
		int pointer = 0;

		for (int count : counts)
		{
			VertexSkinData skinData = VertexSkinData();

			for (int i = 0; i < count; i++)
			{
				int jointId = stoi(rawData[pointer++]);
				int weightId = stoi(rawData[pointer++]);
				skinData.AddJointEffect(jointId, weights[weightId]);
			}

			skinData.LimitJointNumber(m_maxWeights);
			skinningData.push_back(&skinData);
		}

		return skinningData;
	}
}
