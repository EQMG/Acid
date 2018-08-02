#include "Keyframe.hpp"

namespace acid
{
	Keyframe::Keyframe(const float &timeStamp, const std::map<std::string, JointTransform *> &pose) :
		m_timeStamp(timeStamp),
		m_pose(pose)
	{
	}

	Keyframe::Keyframe(const KeyframeData &data) :
		m_timeStamp(data.GetTime())
	{
		for (auto &jointData : data.GetJointTransforms())
		{
			m_pose.emplace(jointData->GetJointNameId(), new JointTransform(*jointData));
		}
	}

	Keyframe::~Keyframe()
	{
		for (auto &transform : m_pose)
		{
			delete transform.second;
		}
	}
}
