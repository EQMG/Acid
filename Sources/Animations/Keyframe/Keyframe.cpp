#include "Keyframe.hpp"

#include <utility>

namespace acid
{
	Keyframe::Keyframe(const Time &timeStamp, std::map<std::string, JointTransform> pose) :
		m_timeStamp(timeStamp),
		m_pose(std::move(pose))
	{
	}

	void Keyframe::AddJointTransform(const std::string &jointNameId, const Matrix4 &jointLocalTransform)
	{
		m_pose.emplace(jointNameId, jointLocalTransform);
	}
}
