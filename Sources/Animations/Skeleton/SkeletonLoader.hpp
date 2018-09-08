#pragma once

#include <optional>
#include "Animations/Joint/JointData.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT SkeletonLoader
	{
	private:
		Metadata *m_armatureData;

		std::vector<std::string> m_boneOrder;

		uint32_t m_jointCount;
		std::unique_ptr<JointData> m_headJoint;
	public:
		SkeletonLoader(Metadata *libraryControllers, const std::vector<std::string> &boneOrder);

		uint32_t GetJointCount() const { return m_jointCount; }

		JointData *GetHeadJoint() const { return m_headJoint.get(); }
	private:
		JointData *LoadJointData(Metadata *jointNode, const bool &isRoot);
				  
		JointData *ExtractMainJointData(Metadata *jointNode, const bool &isRoot);

		std::optional<uint32_t> GetBoneIndex(const std::string &name);
	};
}
