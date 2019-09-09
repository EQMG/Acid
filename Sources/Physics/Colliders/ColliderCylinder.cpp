#include "ColliderCylinder.hpp"

#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include "Scenes/Entity.hpp"

namespace acid
{
ColliderCylinder::ColliderCylinder(float radius, float height, const Transform &localTransform) :
	Collider{localTransform, GizmoType::Create(Model::Create("Gizmos/Cylinder.obj"), 3.0f, Colour::Yellow)},
	m_shape{std::make_unique<btCylinderShape>(btVector3(radius, height / 2.0f, radius))},
	m_radius{radius},
	m_height{height}
{
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

ColliderCylinder::~ColliderCylinder()
{
}

void ColliderCylinder::Start()
{
}

void ColliderCylinder::Update()
{
	Collider::Update();
}

btCollisionShape *ColliderCylinder::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderCylinder::SetRadius(float radius)
{
	m_radius = radius;
	m_shape->setImplicitShapeDimensions({m_radius, m_height / 2.0f, m_radius});
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

void ColliderCylinder::SetHeight(float height)
{
	m_height = height;
	m_shape->setImplicitShapeDimensions({m_radius, m_height / 2.0f, m_radius});
	m_localTransform.SetLocalScale({m_radius, m_height, m_radius});
}

const Node &operator>>(const Node &node, ColliderCylinder &collider)
{
	node["localTransform"].Get(collider.m_localTransform);
	node["radius"].Get(collider.m_radius);
	node["height"].Get(collider.m_height);
	return node;
}

Node &operator<<(Node &node, const ColliderCylinder &collider)
{
	node["localTransform"].Set(collider.m_localTransform);
	node["radius"].Set(collider.m_radius);
	node["height"].Set(collider.m_height);
	return node;
}
}
