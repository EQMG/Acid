#include "ColliderCone.hpp"

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Scenes/Entity.hpp"

namespace acid
{
ColliderCone::ColliderCone(const float &radius, const float &height, const Transform &localTransform) :
	Collider{localTransform, GizmoType::Create(Model::Create("Gizmos/Cone.obj"), 3.0f, Colour::Green)},
	m_shape{std::make_unique<btConeShape>(radius, height)},
	m_radius{radius},
	m_height{height}
{
	m_localTransform.SetScale({m_radius, m_height, m_radius});
}

ColliderCone::~ColliderCone()
{
}

void ColliderCone::Start()
{
}

void ColliderCone::Update()
{
	Collider::Update();
}

btCollisionShape *ColliderCone::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderCone::SetRadius(const float &radius)
{
	m_radius = radius;
	m_shape->setRadius(m_radius);
	m_localTransform.SetScale({m_radius, m_height, m_radius});
}

void ColliderCone::SetHeight(const float &height)
{
	m_height = height;
	m_shape->setHeight(m_height);
	m_localTransform.SetScale({m_radius, m_height, m_radius});
}

const Metadata &operator>>(const Metadata &metadata, ColliderCone &collider)
{
	metadata.GetChild("localTransform", collider.m_localTransform);
	metadata.GetChild("radius", collider.m_radius);
	metadata.GetChild("height", collider.m_height);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ColliderCone &collider)
{
	metadata.SetChild("localTransform", collider.m_localTransform);
	metadata.SetChild("radius", collider.m_radius);
	metadata.SetChild("height", collider.m_height);
	return metadata;
}
}
