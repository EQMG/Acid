#pragma once

#include "Files/Node.hpp"
#include "VertexWeights.hpp"

namespace acid {
class ACID_ANIMATION_EXPORT SkinLoader {
public:
	SkinLoader(NodeConstView &&libraryControllers, uint32_t maxWeights);

	const std::vector<std::string> &GetJointOrder() const { return jointOrder; }
	const std::vector<VertexWeights> &GetVertexWeights() const { return vertexWeights; }

private:
	void LoadJointsList();
	std::vector<float> LoadWeights() const;
	std::vector<uint32_t> GetEffectiveJointsCounts(const Node &weightsDataNode) const;
	void GetSkinWeights(const Node &weightsDataNode, const std::vector<uint32_t> &counts, const std::vector<float> &weights);

	NodeConstView skinData;
	uint32_t maxWeights;

	std::vector<std::string> jointOrder;
	std::vector<VertexWeights> vertexWeights;
};
}
