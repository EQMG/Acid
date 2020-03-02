#include "ColliderSphere.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>

namespace acid {
ColliderSphere::ColliderSphere(float radius, const Transform &localTransform) :
	//Collider(localTransform),
	shape(std::make_unique<btSphereShape>(radius)),
	radius(radius) {
	this->localTransform = localTransform;
	this->localTransform.SetLocalScale({radius, radius, radius});
}

ColliderSphere::~ColliderSphere() {
}

btCollisionShape *ColliderSphere::GetCollisionShape() const {
	return shape.get();
}

void ColliderSphere::SetRadius(float radius) {
	this->radius = radius;
	shape->setUnscaledRadius(radius);
	localTransform.SetLocalScale({radius, radius, radius});
}

const Node &operator>>(const Node &node, ColliderSphere &collider) {
	node["localTransform"].Get(collider.localTransform);
	node["radius"].Get(collider.radius);
	return node;
}

Node &operator<<(Node &node, const ColliderSphere &collider) {
	node["localTransform"].Set(collider.localTransform);
	node["radius"].Set(collider.radius);
	return node;
}
}
