#include "Collider.hpp"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "Maths/Transform.hpp"
#include "Physics/CollisionObject.hpp"

namespace acid {
Collider::Collider(const Transform &localTransform) :
	m_localTransform(localTransform) {
}

void Collider::SetLocalTransform(const Transform &localTransform) {
	m_localTransform = localTransform;
	m_onTransformChange(this, m_localTransform);
}

btVector3 Collider::Convert(const Vector3f &vector) {
	return {vector.m_x, vector.m_y, vector.m_z};
}

Vector3f Collider::Convert(const btVector3 &vector) {
	return {vector.getX(), vector.getY(), vector.getZ()};
}

btQuaternion Collider::Convert(const Quaternion &quaternion) {
	return {quaternion.m_x, quaternion.m_y, quaternion.m_z, quaternion.m_w};
}

Quaternion Collider::Convert(const btQuaternion &quaternion) {
	return {quaternion.getX(), quaternion.getY(), quaternion.getZ(), quaternion.getW()};
}

btTransform Collider::Convert(const Transform &transform) {
	btQuaternion rotation;
	rotation.setEulerZYX(transform.GetRotation().m_y, transform.GetRotation().m_x, transform.GetRotation().m_z);

	btTransform worldTransform;
	worldTransform.setIdentity();
	worldTransform.setOrigin(Convert(transform.GetPosition()));
	worldTransform.setRotation(rotation);
	return worldTransform;
}

Transform Collider::Convert(const btTransform &transform, const Vector3f &scaling) {
	auto position = transform.getOrigin();
	float yaw, pitch, roll;
	transform.getBasis().getEulerYPR(yaw, pitch, roll);
	return {Convert(position), {pitch, yaw, roll}, scaling};
}
}
