#pragma once

#include <vector>
#include "Files/LoadedValue.hpp"

namespace fl
{
	class FL_EXPORT VertexSkinData
	{
	private:
		std::vector<int> m_jointIds;
		std::vector<float> m_weights;
	public:
		VertexSkinData();

		~VertexSkinData();

		void AddJointEffect(const int &jointId, const float &weight);

		void LimitJointNumber(const unsigned int &max);

		void FillEmptyWeights(const unsigned int &max);

		float SaveTopWeights(std::vector<float> *topWeightsArray);

		void RefillWeightList(const std::vector<float> &topWeights, const float &total);

		void RemoveExcessJointIds(const unsigned int &max);

		std::vector<int> GetJointIds() const { return m_jointIds; }

		std::vector<float> GetWeights() const { return m_weights; }
	};
}
