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
		VertexSkinData() :
			m_jointIds(std::vector<int>()),
			m_weights(std::vector<float>())
		{
		}

		~VertexSkinData()
		{
		}

		void AddJointEffect(const int &jointId, const float &weight)
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

		void LimitJointNumber(const unsigned int &max)
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

	class F_EXPORT SkinningData
	{
	private:
		std::vector<std::string> m_jointOrder;
		std::vector<VertexSkinData *> m_verticesSkinData;
	public:
		SkinningData(const std::vector<std::string> &jointOrder, const std::vector<VertexSkinData *> &verticesSkinData) :
			m_jointOrder(std::vector<std::string>(jointOrder)),
			m_verticesSkinData(std::vector<VertexSkinData *>(verticesSkinData))
		{
		}

		~SkinningData()
		{
		}

		std::vector<std::string> GetJointOrder() const { return m_jointOrder; }

		std::vector<VertexSkinData *> GetVerticesSkinData() const { return m_verticesSkinData; }
	};

	class F_EXPORT SkinLoader
	{
	private:
		LoadedValue *m_skinData;

		int m_maxWeights;

		SkinningData *m_skinningData;
	public:
		SkinLoader(LoadedValue *libraryControllers, const int &maxWeights);

		~SkinLoader();

		SkinningData *GetData() const { return m_skinningData; }

	private:
		std::vector<std::string> LoadJointsList();

		std::vector<float> LoadWeights();

		std::vector<int> GetEffectiveJointsCounts(LoadedValue *weightsDataNode);

		std::vector<VertexSkinData *> GetSkinData(LoadedValue *weightsDataNode, const std::vector<int> &counts, const std::vector<float> &weights);
	};
}
