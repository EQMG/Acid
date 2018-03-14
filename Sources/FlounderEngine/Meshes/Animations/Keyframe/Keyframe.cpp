#include "Keyframe.hpp"

namespace Flounder
{
	Keyframe::Keyframe(const float &timeStamp, const std::map<std::string, JointTransform*> &pose) :
		m_timeStamp(timeStamp),
		m_pose(new std::map<std::string, JointTransform*>(pose))
	{
	}

	Keyframe::Keyframe(const KeyframeData &data) :
		m_timeStamp(data.GetTime()),
		m_pose(new std::map<std::string, JointTransform*>())
	{
		for (auto jointData : data.GetJointTransforms())
		{
			JointTransform *jointTransform = new JointTransform(*jointData);
			m_pose->insert(std::make_pair(jointData->GetJointNameId(), jointTransform));
		}
	}

	Keyframe::~Keyframe()
	{
		for (auto transform : *m_pose)
		{
			delete transform.second;
		}

		delete m_pose;
	}
}
