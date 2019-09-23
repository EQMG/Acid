#include "ColliderSphere.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "Scenes/Entity.hpp"

namespace acid {
ColliderSphere::Registrar<ColliderSphere> ColliderSphere::registered("colliderSphere");

ColliderSphere::ColliderSphere(float radius, const Transform &localTransform) :
	Collider(localTransform, GizmoType::Create(Model::Create("Gizmos/Sphere.obj"), 3.0f, Colour::Blue)),
	m_shape(std::make_unique<btSphereShape>(radius)),
	m_radius(radius) {
	m_localTransform.SetLocalScale({m_radius, m_radius, m_radius});
}

ColliderSphere::~ColliderSphere() {
}

void ColliderSphere::Start() {
}

void ColliderSphere::Update() {
	Collider::Update();
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
