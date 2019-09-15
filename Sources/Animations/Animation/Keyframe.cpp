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
	node.GetChild("timeStamp", keyframe.m_timeStamp);
	node.GetChild("pose", keyframe.m_pose);
	return node;
}

Node &operator<<(Node &node, const Keyframe &keyframe) {
	node.SetChild("timeStamp", keyframe.m_timeStamp);
	node.SetChild("pose", keyframe.m_pose);
	return node;
}
}
