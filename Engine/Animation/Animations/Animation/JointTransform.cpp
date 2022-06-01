#include "JointTransform.hpp"

namespace acid {
JointTransform::JointTransform(const Vector3f &position, const Quaternion &rotation) :
	position(position),
	rotation(rotation) {
}

JointTransform::JointTransform(const Matrix4 &localTransform) :
	position(localTransform[3]),
	rotation(localTransform) {
}

Matrix4 JointTransform::GetLocalTransform() const {
	return Matrix4().Translate(position) * rotation.ToRotationMatrix();
}

JointTransform JointTransform::Interpolate(const JointTransform &frameA, const JointTransform &frameB, float progression) {
	auto position = Interpolate(frameA.GetPosition(), frameB.GetPosition(), progression);
	auto rotation = frameA.GetRotation().Slerp(frameB.GetRotation(), progression);
	return {position, rotation};
}

Vector3f JointTransform::Interpolate(const Vector3f &start, const Vector3f &end, float progression) {
	return start + (end - start) * progression;
}

const Node &operator>>(const Node &node, JointTransform &jointTransform) {
	node["position"].Get(jointTransform.position);
	node["rotation"].Get(jointTransform.rotation);
	return node;
}

Node &operator<<(Node &node, const JointTransform &jointTransform) {
	node["position"].Set(jointTransform.position);
	node["rotation"].Set(jointTransform.rotation);
	return node;
}
}
