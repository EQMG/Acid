#pragma once

#include "Animations/Joint/Joint.hpp"
#include "Files/LoadedValue.hpp"

namespace fl
{
	class FL_EXPORT SkeletonLoader
	{
	private:
		LoadedValue *m_armatureData;

		std::vector<std::string> m_boneOrder;

		int m_jointCount;
		JointData *m_headJoint;
	public:
		SkeletonLoader(LoadedValue *libraryControllers, const std::vector<std::string> &boneOrder);

		~SkeletonLoader();

		int GetJointCount() const { return m_jointCount; }

		JointData *GetHeadJoint() const { return m_headJoint; }
	private:
		JointData *LoadJointData(LoadedValue *jointNode, const bool &isRoot);

		JointData *ExtractMainJointData(LoadedValue *jointNode, const bool &isRoot);

		int GetBoneIndex(const std::string &name);

		Matrix4 ConvertData(const std::vector<std::string> &rawData);
	};
}
