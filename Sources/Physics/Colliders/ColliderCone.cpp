#include "ColliderCone.hpp"

#include <BulletCollision/CollisionShapes/btConeShape.h>

namespace acid {
bool ColliderCone::registered = Register("cone");

ColliderCone::ColliderCone(float radius, float height, const Transform &localTransform) :
	//Collider(localTransform),
	m_shape(std::make_unique<btConeShape>(radius, height)),
	m_radius(radius),
	m_height(height) {
	m_localTransform = localTransform;
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

ColliderCone::~ColliderCone() {
}

btCollisionShape *ColliderCone::GetCollisionShape() const {
	return m_shape.get();
}

void ColliderCone::SetRadius(float radius) {
	m_radius = radius;
	m_shape->setRadius(m_radius);
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

void ColliderCone::SetHeight(float height) {
	m_height = height;
	m_shape->setHeight(m_height);
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

const Node &operator>>(const Node &node, ColliderCone &collider) {
	node["localTransform"].Get(collider.m_localTransform);
	node["radius"].Get(collider.m_radius);
	node["height"].Get(collider.m_height);
	return node;
}

Node &operator<<(Node &node, const ColliderCone &collider) {
	node["localTransform"].Set(collider.m_localTransform);
	node["radius"].Set(collider.m_radius);
	node["height"].Set(collider.m_height);
	return node;
}
}
