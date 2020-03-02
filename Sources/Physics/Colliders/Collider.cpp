#include "Collider.hpp"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "Maths/Transform.hpp"

namespace acid {
Collider::Collider(const Transform &localTransform) :
	localTransform(localTransform) {
}

void Collider::SetLocalTransform(const Transform &localTransform) {
	this->localTransform = localTransform;
	onTransformChange(this, this->localTransform);
}

btVector3 Collider::Convert(const Vector3f &vector) {
	return {vector.x, vector.y, vector.z};
}

Vector3f Collider::Convert(const btVector3 &vector) {
	return {vector.getX(), vector.getY(), vector.getZ()};
}

btQuaternion Collider::Convert(const Quaternion &quaternion) {
	return {quaternion.x, quaternion.y, quaternion.z, quaternion.w};
}

Quaternion Collider::Convert(const btQuaternion &quaternion) {
	return {quaternion.getX(), quaternion.getY(), quaternion.getZ(), quaternion.getW()};
}

btTransform Collider::Convert(const Transform &transform) {
	btQuaternion rotation;
	rotation.setEulerZYX(transform.GetRotation().y, transform.GetRotation().x, transform.GetRotation().z);

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
