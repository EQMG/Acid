#pragma once

#include "Serialized/Metadata.hpp"
#include "Joint.hpp"

namespace acid
{
class ACID_EXPORT SkeletonLoader
{
public:
	SkeletonLoader(const Metadata *libraryControllers, std::vector<std::string> boneOrder, const Matrix4 &correction);

	const uint32_t &GetJointCount() const { return m_jointCount; }

	const Joint &GetHeadJoint() const { return m_headJoint; }

private:
	Joint LoadJointData(const Metadata *jointNode, const bool &isRoot);

	Joint ExtractMainJointData(const Metadata *jointNode, const bool &isRoot);

	std::optional<uint32_t> GetBoneIndex(const std::string &name);

	const Metadata *m_armatureData{};
	std::vector<std::string> m_boneOrder;
	Matrix4 m_correction;

	uint32_t m_jointCount{};
	Joint m_headJoint;
};
}
