#include "Keyframe.hpp"

namespace acid {
Keyframe::Keyframe(const Time &timeStamp, std::map<std::string, JointTransform> pose) :
	m_timeStamp(timeStamp),
	m_pose(std::move(pose)) {
}

void Keyframe::AddJointTransform(const std::string &jointNameId, const Matrix4 &jointLocalTransform) {
	m_pose.emplace(jointNameId, jointLocalTransform);
}

const Node &operator>>(const Node &node, Keyframe &keyframe) {
	node["timeStamp"].Get(keyframe.m_timeStamp);
	node["pose"].Get(keyframe.m_pose);
	return node;
}

Node &operator<<(Node &node, const Keyframe &keyframe) {
	node["timeStamp"].Set(keyframe.m_timeStamp);
	node["pose"].Set(keyframe.m_pose);
	return node;
}
}
