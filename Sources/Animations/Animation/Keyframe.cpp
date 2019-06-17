#include "Keyframe.hpp"

namespace acid
{
Keyframe::Keyframe(const Time &timeStamp, std::map<std::string, JointTransform> pose) :
	m_timeStamp{timeStamp},
	m_pose{std::move(pose)}
{
}

void Keyframe::AddJointTransform(const std::string &jointNameId, const Matrix4 &jointLocalTransform)
{
	m_pose.emplace(jointNameId, jointLocalTransform);
}

const Metadata &operator>>(const Metadata &metadata, Keyframe &keyframe)
{
	metadata.GetChild("timeStamp", keyframe.m_timeStamp);
	metadata.GetChild("pose", keyframe.m_pose);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Keyframe &keyframe)
{
	metadata.SetChild("timeStamp", keyframe.m_timeStamp);
	metadata.SetChild("pose", keyframe.m_pose);
	return metadata;
}
}
