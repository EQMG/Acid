#include "VertexSkinData.hpp"

namespace fl
{
	VertexSkinData::VertexSkinData() :
		m_jointIds(std::vector<int>()),
		m_weights(std::vector<float>())
	{
	}

	VertexSkinData::~VertexSkinData()
	{
	}

	void VertexSkinData::AddJointEffect(const int &jointId, const float &weight)
	{
		for (unsigned int i = 0; i < m_weights.size(); i++)
		{
			if (weight > m_weights.at(i))
			{
				m_jointIds.insert(m_jointIds.begin() + i, jointId);
				m_weights.insert(m_weights.begin() + i, weight);
				return;
			}
		}

		m_jointIds.emplace_back(jointId);
		m_weights.emplace_back(weight);
	}

	void VertexSkinData::LimitJointNumber(const unsigned int &max)
	{
		if (m_jointIds.size() > max)
		{
			std::vector<float> topWeights = std::vector<float>(max);
			float total = SaveTopWeights(&topWeights);
			RefillWeightList(topWeights, total);
			RemoveExcessJointIds(max);
		}
		else if (m_jointIds.size() < max)
		{
			FillEmptyWeights(max);
		}
	}

	void VertexSkinData::FillEmptyWeights(const unsigned int &max)
	{
		while (m_jointIds.size() < max)
		{
			m_jointIds.emplace_back(0);
			m_weights.emplace_back(0.0f);
		}
	}

	float VertexSkinData::SaveTopWeights(std::vector<float> *topWeightsArray)
	{
		float total = 0.0f;

		for (unsigned int i = 0; i < topWeightsArray->size(); i++)
		{
			topWeightsArray->at(i) = m_weights.at(i);
			total += topWeightsArray->at(i);
		}

		return total;
	}

	void VertexSkinData::RefillWeightList(const std::vector<float> &topWeights, const float &total)
	{
		m_weights.clear();

		for (float topWeight : topWeights)
		{
			m_weights.emplace_back(topWeight / total);
		}
	}

	void VertexSkinData::RemoveExcessJointIds(const unsigned int &max)
	{
		while (m_jointIds.size() > max)
		{
			m_jointIds.erase(m_jointIds.end() - 1);
		}
	}
}
