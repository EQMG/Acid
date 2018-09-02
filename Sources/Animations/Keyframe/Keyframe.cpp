#include "Keyframe.hpp"

namespace acid
{
	Keyframe::Keyframe(const float &timeStamp, const std::map<std::string, JointTransform> &pose) :
		m_timeStamp(timeStamp),
		m_pose(pose)
	{
	}

	Keyframe::Keyframe(const KeyframeData &data) :
		m_timeStamp(data.GetTime()),
		m_pose(std::map<std::string, JointTransform>())
	{
		for (auto &jointData : data.GetJointTransforms())
		{
			m_pose.emplace(jointData.GetJointNameId(), JointTransform(jointData));
		}
	}

	Keyframe::~Keyframe()
	{
	}
}
