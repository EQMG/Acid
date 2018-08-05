#include "VertexSkinData.hpp"

namespace acid
{
	VertexSkinData::VertexSkinData()
	{
	}

	VertexSkinData::~VertexSkinData()
	{
	}

	void VertexSkinData::AddJointEffect(const int &jointId, const float &weight)
	{
		for (uint32_t i = 0; i < m_weights.size(); i++)
		{
			if (weight > m_weights.at(i))
			{
				m_jointIds[i] = jointId;
				m_weights[i] = weight;
				return;
			}
		}

		m_jointIds.emplace_back(jointId);
		m_weights.emplace_back(weight);
	}

	void VertexSkinData::LimitJointNumber(const uint32_t &max)
	{
		if (m_jointIds.size() > max)
		{
			auto topWeights = std::vector<float>(max);
			float total = SaveTopWeights(topWeights);
			RefillWeightList(topWeights, total);
			RemoveExcessJointIds(max);
		}
		else if (m_jointIds.size() < max)
		{
			FillEmptyWeights(max);
		}
	}

	void VertexSkinData::FillEmptyWeights(const uint32_t &max)
	{
		while (m_jointIds.size() < max)
		{
			m_jointIds.emplace_back(0);
			m_weights.emplace_back(0.0f);
		}
	}

	float VertexSkinData::SaveTopWeights(std::vector<float> &topWeightsArray)
	{
		float total = 0.0f;

		for (uint32_t i = 0; i < topWeightsArray.size(); i++)
		{
			topWeightsArray[i] = m_weights.at(i);
			total += topWeightsArray.at(i);
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

	void VertexSkinData::RemoveExcessJointIds(const uint32_t &max)
	{
		while (m_jointIds.size() > max)
		{
			m_jointIds.pop_back();
		}
	}
}
