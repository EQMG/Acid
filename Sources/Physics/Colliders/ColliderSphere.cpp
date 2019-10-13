#include "ColliderSphere.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>

namespace acid {
bool ColliderSphere::registered = Register("sphere");

ColliderSphere::ColliderSphere(float radius, const Transform &localTransform) :
	//Collider(localTransform),
	m_shape(std::make_unique<btSphereShape>(radius)),
	m_radius(radius) {
	m_localTransform = localTransform;
	m_localTransform.SetLocalScale({m_radius, m_radius, m_radius});
}

ColliderSphere::~ColliderSphere() {
}

btCollisionShape *ColliderSphere::GetCollisionShape() const {
	return m_shape.get();
}

void ColliderSphere::SetRadius(float radius) {
	m_radius = radius;
	m_shape->setUnscaledRadius(m_radius);
	m_localTransform.SetLocalScale({m_radius, m_radius, m_radius});
}

const Node &operator>>(const Node &node, ColliderSphere &collider) {
	node["localTransform"].Get(collider.m_localTransform);
	node["radius"].Get(collider.m_radius);
	return node;
}

Node &operator<<(Node &node, const ColliderSphere &collider) {
	node["localTransform"].Set(collider.m_localTransform);
	node["radius"].Set(collider.m_radius);
	return node;
}
}
