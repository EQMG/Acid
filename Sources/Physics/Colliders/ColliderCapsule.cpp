#include "ColliderCapsule.hpp"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include "Scenes/Entity.hpp"

namespace acid
{
ColliderCapsule::ColliderCapsule(float radius, float height, const Transform &localTransform) :
	Collider(localTransform, GizmoType::Create(Model::Create("Gizmos/Capsule.obj"), 3.0f, Colour::Fuchsia)),
	m_shape(std::make_unique<btCapsuleShape>(radius, height)),
	m_radius(radius),
	m_height(height)
{
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

ColliderCapsule::~ColliderCapsule()
{
}

void ColliderCapsule::Start()
{
}

void ColliderCapsule::Update()
{
	Collider::Update();
}

btCollisionShape *ColliderCapsule::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderCapsule::SetRadius(float radius)
{
	m_radius = radius;
	m_shape->setImplicitShapeDimensions({m_radius, 0.5f * m_height, m_radius});
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

void ColliderCapsule::SetHeight(float height)
{
	m_height = height;
	m_shape->setImplicitShapeDimensions({m_radius, 0.5f * m_height, m_radius});
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

const Node &operator>>(const Node &node, ColliderCapsule &collider)
{
	node["localTransform"].Get(collider.m_localTransform);
	node["radius"].Get(collider.m_radius);
	node["height"].Get(collider.m_height);
	return node;
}

Node &operator<<(Node &node, const ColliderCapsule &collider)
{
	node["localTransform"].Set(collider.m_localTransform);
	node["radius"].Set(collider.m_radius);
	node["height"].Set(collider.m_height);
	return node;
}
}
