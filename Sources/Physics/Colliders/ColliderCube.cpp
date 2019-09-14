#include "ColliderCube.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Scenes/Entity.hpp"

namespace acid
{
ColliderCube::ColliderCube(const Vector3f &extents, const Transform &localTransform) :
	Collider(localTransform, GizmoType::Create(Model::Create("Gizmos/Cube.obj"), 3.0f, Colour::Red)),
	m_shape(std::make_unique<btBoxShape>(Convert(extents / 2.0f))),
	m_extents(extents)
{
	m_localTransform.SetLocalScale(m_extents);
}

ColliderCube::~ColliderCube()
{
}

void ColliderCube::Start()
{
}

void ColliderCube::Update()
{
	Collider::Update();
}

btCollisionShape *ColliderCube::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderCube::SetExtents(const Vector3f &extents)
{
	m_extents = extents;
	m_shape->setImplicitShapeDimensions(Convert(m_extents));
	m_localTransform.SetLocalScale(m_extents);
}

const Node &operator>>(const Node &node, ColliderCube &collider)
{
	node["localTransform"].Get(collider.m_localTransform);
	node["extents"].Get(collider.m_extents);
	return node;
}

Node &operator<<(Node &node, const ColliderCube &collider)
{
	node["localTransform"].Set(collider.m_localTransform);
	node["extents"].Set(collider.m_extents);
	return node;
}
}
