#include "SphereCollider.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>

namespace acid {
const bool SphereCollider::Registered = Register("sphere");

SphereCollider::SphereCollider(float radius, const Transform &localTransform) :
	//Collider(localTransform),
	shape(std::make_unique<btSphereShape>(radius)),
	radius(radius) {
	this->localTransform = localTransform;
	this->localTransform.SetLocalScale({radius, radius, radius});
}

SphereCollider::~SphereCollider() {
}

btCollisionShape *SphereCollider::GetCollisionShape() const {
	return shape.get();
}

void SphereCollider::SetRadius(float radius) {
	this->radius = radius;
	shape->setUnscaledRadius(radius);
	localTransform.SetLocalScale({radius, radius, radius});
}

const Node &operator>>(const Node &node, SphereCollider &collider) {
	node["localTransform"].Get(collider.localTransform);
	node["radius"].Get(collider.radius);
	return node;
}

Node &operator<<(Node &node, const SphereCollider &collider) {
	node["localTransform"].Set(collider.localTransform);
	node["radius"].Set(collider.radius);
	return node;
}
}
