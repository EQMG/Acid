#include "VertexSkinData.hpp"

namespace Flounder
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
			if (weight > m_weights[i])
			{
				m_jointIds[i] = jointId;
				m_weights[i] = weight;
				return;
			}
		}

		m_jointIds.push_back(jointId);
		m_weights.push_back(weight);
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
}
