#pragma once

#include <vector>
#include "Files/LoadedValue.hpp"

namespace acid
{
	class ACID_EXPORT VertexSkinData
	{
	private:
		std::vector<int> m_jointIds;
		std::vector<float> m_weights;
	public:
		VertexSkinData();

		~VertexSkinData();

		void AddJointEffect(const int &jointId, const float &weight);

		void LimitJointNumber(const uint32_t &max);

		void FillEmptyWeights(const uint32_t &max);

		float SaveTopWeights(std::vector<float> &topWeightsArray);

		void RefillWeightList(const std::vector<float> &topWeights, const float &total);

		void RemoveExcessJointIds(const uint32_t &max);

		std::vector<int> GetJointIds() const { return m_jointIds; }

		std::vector<float> GetWeights() const { return m_weights; }
	};
}
