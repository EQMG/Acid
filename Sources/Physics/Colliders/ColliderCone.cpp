#include "ColliderCone.hpp"

#include <BulletCollision/CollisionShapes/btConeShape.h>

namespace acid {
ColliderCone::ColliderCone(float radius, float height, const Transform &localTransform) :
	//Collider(localTransform),
	shape(std::make_unique<btConeShape>(radius, height)),
	radius(radius),
	height(height) {
	this->localTransform = localTransform;
	this->localTransform.SetLocalScale({radius, height, radius});
}

ColliderCone::~ColliderCone() {
}

btCollisionShape *ColliderCone::GetCollisionShape() const {
	return shape.get();
}

void ColliderCone::SetRadius(float radius) {
	this->radius = radius;
	shape->setRadius(radius);
	localTransform.SetLocalScale({radius, height, radius});
}

void ColliderCone::SetHeight(float height) {
	this->height = height;
	shape->setHeight(height);
	localTransform.SetLocalScale({radius, height, radius});
}

const Node &operator>>(const Node &node, ColliderCone &collider) {
	node["localTransform"].Get(collider.localTransform);
	node["radius"].Get(collider.radius);
	node["height"].Get(collider.height);
	return node;
}

Node &operator<<(Node &node, const ColliderCone &collider) {
	node["localTransform"].Set(collider.localTransform);
	node["radius"].Set(collider.radius);
	node["height"].Set(collider.height);
	return node;
}
}
