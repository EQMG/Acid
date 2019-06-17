#include "ColliderCube.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Scenes/Entity.hpp"

namespace acid
{
ColliderCube::ColliderCube(const Vector3f &extents, const Transform &localTransform) :
	Collider{localTransform, GizmoType::Create(Model::Create("Gizmos/Cube.obj"), 3.0f, Colour::Red)},
	m_shape{std::make_unique<btBoxShape>(Convert(extents / 2.0f))},
	m_extents{extents}
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

const Metadata &operator>>(const Metadata &metadata, ColliderCube &collider)
{
	metadata.GetChild("localTransform", collider.m_localTransform);
	metadata.GetChild("extents", collider.m_extents);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ColliderCube &collider)
{
	metadata.SetChild("localTransform", collider.m_localTransform);
	metadata.SetChild("extents", collider.m_extents);
	return metadata;
}
}
