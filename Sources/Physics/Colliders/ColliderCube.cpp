#include "ColliderCube.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace acid {
ColliderCube::ColliderCube(const Vector3f &extents, const Transform &localTransform) :
	//Collider(localTransform),
	shape(std::make_unique<btBoxShape>(Convert(extents / 2.0f))),
	extents(extents) {
	this->localTransform = localTransform;
	this->localTransform.SetLocalScale(extents);
}

ColliderCube::~ColliderCube() {
}

btCollisionShape *ColliderCube::GetCollisionShape() const {
	return shape.get();
}

void ColliderCube::SetExtents(const Vector3f &extents) {
	this->extents = extents;
	shape->setImplicitShapeDimensions(Convert(extents));
	localTransform.SetLocalScale(extents);
}

const Node &operator>>(const Node &node, ColliderCube &collider) {
	node["localTransform"].Get(collider.localTransform);
	node["extents"].Get(collider.extents);
	return node;
}

Node &operator<<(Node &node, const ColliderCube &collider) {
	node["localTransform"].Set(collider.localTransform);
	node["extents"].Set(collider.extents);
	return node;
}
}
