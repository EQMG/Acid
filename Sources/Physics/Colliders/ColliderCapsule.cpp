#include "ColliderCapsule.hpp"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

namespace acid {
ColliderCapsule::ColliderCapsule(float radius, float height, const Transform &localTransform) :
	//Collider(localTransform),
	shape(std::make_unique<btCapsuleShape>(radius, height)),
	radius(radius),
	height(height) {
	this->localTransform = localTransform;
	this->localTransform.SetLocalScale({radius, height, radius});
}

ColliderCapsule::~ColliderCapsule() {
}

btCollisionShape *ColliderCapsule::GetCollisionShape() const {
	return shape.get();
}

void ColliderCapsule::SetRadius(float radius) {
	this->radius = radius;
	shape->setImplicitShapeDimensions({radius, 0.5f * height, radius});
	localTransform.SetLocalScale({radius, height, radius});
}

void ColliderCapsule::SetHeight(float height) {
	this->height = height;
	shape->setImplicitShapeDimensions({radius, 0.5f * height, radius});
	localTransform.SetLocalScale({radius, height, radius});
}

const Node &operator>>(const Node &node, ColliderCapsule &collider) {
	node["localTransform"].Get(collider.localTransform);
	node["radius"].Get(collider.radius);
	node["height"].Get(collider.height);
	return node;
}

Node &operator<<(Node &node, const ColliderCapsule &collider) {
	node["localTransform"].Set(collider.localTransform);
	node["radius"].Set(collider.radius);
	node["height"].Set(collider.height);
	return node;
}
}
