#pragma once

#include <optional>
#include "Animations/Joint/JointData.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT SkeletonLoader
	{
	private:
		std::shared_ptr<Metadata> m_armatureData;

		std::vector<std::string> m_boneOrder;

		uint32_t m_jointCount;
		std::shared_ptr<JointData> m_headJoint;
	public:
		SkeletonLoader(const std::shared_ptr<Metadata> &libraryControllers, const std::vector<std::string> &boneOrder);

		uint32_t GetJointCount() const { return m_jointCount; }

		std::shared_ptr<JointData> GetHeadJoint() const { return m_headJoint; }
	private:
		std::shared_ptr<JointData> LoadJointData(const std::shared_ptr<Metadata> &jointNode, const bool &isRoot);

		std::shared_ptr<JointData> ExtractMainJointData(const std::shared_ptr<Metadata> &jointNode, const bool &isRoot);

		std::optional<uint32_t> GetBoneIndex(const std::string &name);
	};
}
