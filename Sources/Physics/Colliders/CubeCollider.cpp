#include "CubeCollider.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace acid {
const bool CubeCollider::Registered = Register("cube");

CubeCollider::CubeCollider(const Vector3f &extents, const Transform &localTransform) :
	//Collider(localTransform),
	shape(std::make_unique<btBoxShape>(Convert(extents / 2.0f))),
	extents(extents) {
	this->localTransform = localTransform;
	this->localTransform.SetLocalScale(extents);
}

CubeCollider::~CubeCollider() {
}

btCollisionShape *CubeCollider::GetCollisionShape() const {
	return shape.get();
}

void CubeCollider::SetExtents(const Vector3f &extents) {
	this->extents = extents;
	shape->setImplicitShapeDimensions(Convert(extents));
	localTransform.SetLocalScale(extents);
}

const Node &operator>>(const Node &node, CubeCollider &collider) {
	node["localTransform"].Get(collider.localTransform);
	node["extents"].Get(collider.extents);
	return node;
}

Node &operator<<(Node &node, const CubeCollider &collider) {
	node["localTransform"].Set(collider.localTransform);
	node["extents"].Set(collider.extents);
	return node;
}
}
