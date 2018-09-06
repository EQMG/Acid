#pragma once

#include <cstdint>
#include <vector>
#include "Engine/Exports.hpp"

namespace acid
{
	class ACID_EXPORT VertexSkinData
	{
	private:
		std::vector<uint32_t> m_jointIds;
		std::vector<float> m_weights;
	public:
		VertexSkinData();

		void AddJointEffect(const uint32_t &jointId, const float &weight);

		void LimitJointNumber(const uint32_t &max);

		void FillEmptyWeights(const uint32_t &max);

		float SaveTopWeights(std::vector<float> &topWeightsArray);

		void RefillWeightList(const std::vector<float> &topWeights, const float &total);

		void RemoveExcessJointIds(const uint32_t &max);

		std::vector<uint32_t> GetJointIds() const { return m_jointIds; }

		std::vector<float> GetWeights() const { return m_weights; }
	};
}
