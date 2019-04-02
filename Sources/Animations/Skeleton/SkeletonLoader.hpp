#pragma once

#include "Serialized/Metadata.hpp"
#include "Animations/Joint/Joint.hpp"

namespace acid
{
class ACID_EXPORT SkeletonLoader
{
public:
	SkeletonLoader(const Metadata *libraryControllers, std::vector<std::string> boneOrder);

	const uint32_t &GetJointCount() const { return m_jointCount; }

	const JointData *GetHeadJoint() const { return m_headJoint.get(); }

private:
	JointData *LoadJointData(const Metadata *jointNode, const bool &isRoot);

	JointData *ExtractMainJointData(const Metadata *jointNode, const bool &isRoot);

	std::optional<uint32_t> GetBoneIndex(const std::string &name);

	const Metadata *m_armatureData;
	std::vector<std::string> m_boneOrder;

	uint32_t m_jointCount;
	std::unique_ptr<JointData> m_headJoint;
};
}
