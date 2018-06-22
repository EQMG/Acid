#pragma once

#include "Animations/Joint/Joint.hpp"
#include "Files/LoadedValue.hpp"

namespace fl
{
	class FL_EXPORT SkeletonLoader
	{
	private:
		std::shared_ptr<LoadedValue> m_armatureData;

		std::vector<std::string> m_boneOrder;

		int m_jointCount;
		JointData *m_headJoint;
	public:
		SkeletonLoader(std::shared_ptr<LoadedValue> libraryControllers, const std::vector<std::string> &boneOrder);

		~SkeletonLoader();

		int GetJointCount() const { return m_jointCount; }

		JointData *GetHeadJoint() const { return m_headJoint; }
	private:
		JointData *LoadJointData(std::shared_ptr<LoadedValue> jointNode, const bool &isRoot);

		JointData *ExtractMainJointData(std::shared_ptr<LoadedValue> jointNode, const bool &isRoot);

		int GetBoneIndex(const std::string &name);

		Matrix4 ConvertData(const std::vector<std::string> &rawData);
	};
}
