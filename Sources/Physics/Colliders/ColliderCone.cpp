#include "ColliderCone.hpp"

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Scenes/Entity.hpp"
#include "Physics/CollisionObject.hpp"

namespace acid {
bool ColliderCone::registered = Register("colliderCone");

ColliderCone::ColliderCone(float radius, float height, const Transform &localTransform) :
	Collider(localTransform, GizmoType::Create(Model::Create("Gizmos/Cone.obj"), 3.0f, Colour::Green)),
	m_shape(std::make_unique<btConeShape>(radius, height)),
	m_radius(radius),
	m_height(height) {
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

ColliderCone::~ColliderCone() {
}

void ColliderCone::Start() {
}

void ColliderCone::Update() {
	//Collider::Update();
}

btCollisionShape *ColliderCone::GetCollisionShape() const {
	return m_shape.get();
}

void ColliderCone::SetLocalTransform(const Transform &localTransform) {
	m_localTransform = localTransform;
	if (auto collisionObject = GetEntity()->GetComponent<CollisionObject>())
		collisionObject->SetChildTransform(this, m_localTransform);
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
