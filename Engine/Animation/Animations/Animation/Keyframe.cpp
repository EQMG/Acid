#include "Keyframe.hpp"

namespace acid {
Keyframe::Keyframe(const Time &timeStamp, std::map<std::string, JointTransform> pose) :
	timeStamp(timeStamp),
	pose(std::move(pose)) {
}

void Keyframe::AddJointTransform(const std::string &jointNameId, const Matrix4 &jointLocalTransform) {
	pose.emplace(jointNameId, jointLocalTransform);
}

const Node &operator>>(const Node &node, Keyframe &keyframe) {
	node["timeStamp"].Get(keyframe.timeStamp);
	node["pose"].Get(keyframe.pose);
	return node;
}

Node &operator<<(Node &node, const Keyframe &keyframe) {
	node["timeStamp"].Set(keyframe.timeStamp);
	node["pose"].Set(keyframe.pose);
	return node;
}
}
