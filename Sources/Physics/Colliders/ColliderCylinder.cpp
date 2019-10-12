#include "ColliderCylinder.hpp"

#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include "Physics/CollisionObject.hpp"

namespace acid {
bool ColliderCylinder::registered = Register("cylinder");

ColliderCylinder::ColliderCylinder(float radius, float height, const Transform &localTransform) :
	//Collider(localTransform),
	m_shape(std::make_unique<btCylinderShape>(btVector3(radius, height / 2.0f, radius))),
	m_radius(radius),
	m_height(height) {
	m_localTransform = localTransform;
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

ColliderCylinder::~ColliderCylinder() {
}

btCollisionShape *ColliderCylinder::GetCollisionShape() const {
	return m_shape.get();
}

void ColliderCylinder::SetRadius(float radius) {
	m_radius = radius;
	m_shape->setImplicitShapeDimensions({m_radius, m_height / 2.0f, m_radius});
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

void ColliderCylinder::SetHeight(float height) {
	m_height = height;
	m_shape->setImplicitShapeDimensions({m_radius, m_height / 2.0f, m_radius});
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

const Node &operator>>(const Node &node, ColliderCylinder &collider) {
	node["localTransform"].Get(collider.m_localTransform);
	node["radius"].Get(collider.m_radius);
	node["height"].Get(collider.m_height);
	return node;
}

Node &operator<<(Node &node, const ColliderCylinder &collider) {
	node["localTransform"].Set(collider.m_localTransform);
	node["radius"].Set(collider.m_radius);
	node["height"].Set(collider.m_height);
	return node;
}
}
