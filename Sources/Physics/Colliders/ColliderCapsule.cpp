#include "ColliderCapsule.hpp"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include "Scenes/Entity.hpp"

namespace acid
{
ColliderCapsule::ColliderCapsule(const float &radius, const float &height, const Transform &localTransform) :
	Collider{localTransform, GizmoType::Create(Model::Create("Gizmos/Capsule.obj"), 3.0f, Colour::Fuchsia)},
	m_shape{std::make_unique<btCapsuleShape>(radius, height)},
	m_radius{radius},
	m_height{height}
{
	m_localTransform.SetScaling({m_radius, m_height, m_radius});
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

void ColliderCapsule::SetRadius(const float &radius)
{
	m_radius = radius;
	m_shape->setImplicitShapeDimensions({m_radius, 0.5f * m_height, m_radius});
	m_localTransform.SetScaling({m_radius, m_height, m_radius});
}

void ColliderCapsule::SetHeight(const float &height)
{
	m_height = height;
	m_shape->setImplicitShapeDimensions({m_radius, 0.5f * m_height, m_radius});
	m_localTransform.SetScaling({m_radius, m_height, m_radius});
}

const Metadata &operator>>(const Metadata &metadata, ColliderCapsule &collider)
{
	metadata.GetChild("Local Transform", collider.m_localTransform);
	metadata.GetChild("Radius", collider.m_radius);
	metadata.GetChild("Height", collider.m_height);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ColliderCapsule &collider)
{
	metadata.SetChild("Local Transform", collider.m_localTransform);
	metadata.SetChild("Radius", collider.m_radius);
	metadata.SetChild("Height", collider.m_height);
	return metadata;
}
}
