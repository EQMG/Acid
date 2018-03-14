#pragma once

#include "../Joint/Joint.hpp"
#include "../../../Files/LoadedValue.hpp"

namespace Flounder
{
	class F_EXPORT SkeletonData
	{
	private:
		int m_jointCount;
		JointData *m_headJoint;
	public:
		SkeletonData(const int &jointCount, JointData *headJoint) :
			m_jointCount(jointCount),
			m_headJoint(headJoint)
		{
		}

		~SkeletonData()
		{
		}

		int GetJointCount() const { return m_jointCount; }

		JointData *GetHeadJoint() const { return m_headJoint; }
	};

	class F_EXPORT SkeletonLoader
	{
	private:
		LoadedValue *m_armatureData;

		std::vector<std::string> *m_boneOrder;
		int m_jointCount;

		SkeletonData *m_skeletonData;
	public:
		SkeletonLoader(LoadedValue *parent, std::vector<std::string> *boneOrder);

		~SkeletonLoader();

		SkeletonData *GetData() const { return m_skeletonData; }

	private:
		JointData *LoadJointData(LoadedValue *jointNode, const bool &isRoot);

		JointData *ExtractMainJointData(LoadedValue *jointNode, const bool &isRoot);

		int GetBoneIndex(const std::string &name);

		Matrix4 ConvertData(const std::vector<std::string> &rawData);
	};
}
