#include "CylinderCollider.hpp"

#include <BulletCollision/CollisionShapes/btCylinderShape.h>

namespace acid {
CylinderCollider::CylinderCollider(float radius, float height, const Transform &localTransform) :
	//Collider(localTransform),
	shape(std::make_unique<btCylinderShape>(btVector3(radius, height / 2.0f, radius))),
	radius(radius),
	height(height) {
	this->localTransform = localTransform;
	this->localTransform.SetLocalScale({radius, height, radius});
}

CylinderCollider::~CylinderCollider() {
}

btCollisionShape *CylinderCollider::GetCollisionShape() const {
	return shape.get();
}

void CylinderCollider::SetRadius(float radius) {
	this->radius = radius;
	shape->setImplicitShapeDimensions({radius, height / 2.0f, radius});
	localTransform.SetLocalScale({radius, height, radius});
}

void CylinderCollider::SetHeight(float height) {
	this->height = height;
	shape->setImplicitShapeDimensions({radius, height / 2.0f, radius});
	localTransform.SetLocalScale({radius, height, radius});
}

const Node &operator>>(const Node &node, CylinderCollider &collider) {
	node["localTransform"].Get(collider.localTransform);
	node["radius"].Get(collider.radius);
	node["height"].Get(collider.height);
	return node;
}

Node &operator<<(Node &node, const CylinderCollider &collider) {
	node["localTransform"].Set(collider.localTransform);
	node["radius"].Set(collider.radius);
	node["height"].Set(collider.height);
	return node;
}
}
