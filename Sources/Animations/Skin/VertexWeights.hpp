#pragma once

#include "StdAfx.hpp"

namespace acid
{
class ACID_EXPORT VertexWeights
{
public:
	void AddJointEffect(const uint32_t &jointId, const float &weight);

	void LimitJointNumber(const uint32_t &max);

	void FillEmptyWeights(const uint32_t &max);

	float SaveTopWeights(std::vector<float> &topWeightsArray);

	void RefillWeightList(const std::vector<float> &topWeights, const float &total);

	void RemoveExcessJointIds(const uint32_t &max);

	const std::vector<uint32_t> &GetJointIds() const { return m_jointIds; }

	const std::vector<float> &GetWeights() const { return m_weights; }

private:
	std::vector<uint32_t> m_jointIds;
	std::vector<float> m_weights;
};
}
