#pragma once

#include "StdAfx.hpp"

namespace acid {
class ACID_EXPORT VertexWeights {
public:
	void AddJointEffect(uint32_t jointId, float jointWeight);
	void LimitJointNumber(uint32_t max);
	void FillEmptyWeights(uint32_t max);
	float SaveTopWeights(std::vector<float> &topWeightsArray);
	void RefillWeightList(const std::vector<float> &topWeights, float total);
	void RemoveExcessJointIds(uint32_t max);

	const std::vector<uint32_t> &GetJointIds() const { return m_jointIds; }
	const std::vector<float> &GetWeights() const { return m_weights; }

private:
	std::vector<uint32_t> m_jointIds;
	std::vector<float> m_weights;
};
}
