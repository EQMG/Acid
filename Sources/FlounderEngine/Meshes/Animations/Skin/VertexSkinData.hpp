#pragma once

#include <vector>
#include "../../../Files/LoadedValue.hpp"

namespace Flounder
{
	class F_EXPORT VertexSkinData
	{
	private:
		std::vector<int> m_jointIds;
		std::vector<float> m_weights;
	public:
		VertexSkinData();

		~VertexSkinData();

		void AddJointEffect(const int &jointId, const float &weight);

		void LimitJointNumber(const unsigned int &max);

		void FillEmptyWeights(const unsigned int &max)
		{
			while (m_jointIds.size() < max)
			{
				m_jointIds.push_back(0);
				m_weights.push_back(0.0f);
			}
		}

		float SaveTopWeights(std::vector<float> *topWeightsArray)
		{
			float total = 0.0f;

			for (unsigned int i = 0; i < topWeightsArray->size(); i++)
			{
				(*topWeightsArray)[i] = m_weights[i];
				total += (*topWeightsArray)[i];
			}

			return total;
		}

		void RefillWeightList(const std::vector<float> &topWeights, const float &total)
		{
			m_weights.clear();

			for (float topWeight : topWeights)
			{
				m_weights.push_back(topWeight / total);
			}
		}

		void RemoveExcessJointIds(const unsigned int &max)
		{
			while (m_jointIds.size() > max)
			{
				m_jointIds.erase(m_jointIds.end() - 1);
			}
		}

		std::vector<int> GetJointIds() const { return m_jointIds; }

		std::vector<float> GetWeights() const { return m_weights; }
	};
}
