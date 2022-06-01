#pragma once

#include "Files/Node.hpp"
#include "Joint.hpp"

namespace acid {
class ACID_ANIMATION_EXPORT SkeletonLoader {
public:
	SkeletonLoader(NodeConstView &&libraryControllers, std::vector<std::string> boneOrder, const Matrix4 &correction);

	uint32_t GetJointCount() const { return jointCount; }
	const Joint &GetHeadJoint() const { return headJoint; }

private:
	Joint LoadJointData(const Node &jointNode, bool isRoot);
	Joint ExtractMainJointData(const Node &jointNode, bool isRoot);
	std::optional<uint32_t> GetBoneIndex(const std::string &name) const;

	NodeConstView armatureData;
	std::vector<std::string> boneOrder;
	Matrix4 correction;

	uint32_t jointCount = 0;
	Joint headJoint;
};
}
