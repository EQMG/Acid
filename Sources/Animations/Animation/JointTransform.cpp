#include "JointTransform.hpp"

namespace acid {
JointTransform::JointTransform(const Vector3f &position, const Quaternion &rotation) :
	m_position(position),
	m_rotation(rotation) {
}

JointTransform::JointTransform(const Matrix4 &localTransform) :
	m_position(localTransform[3]),
	m_rotation(localTransform) {
}

Matrix4 JointTransform::GetLocalTransform() const {
	return Matrix4().Translate(m_position) * m_rotation.ToRotationMatrix();
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
	node.GetChild("position", jointTransform.m_position);
	node.GetChild("rotation", jointTransform.m_rotation);
	return node;
}

Node &operator<<(Node &node, const JointTransform &jointTransform) {
	node.SetChild("position", jointTransform.m_position);
	node.SetChild("rotation", jointTransform.m_rotation);
	return node;
}
}
