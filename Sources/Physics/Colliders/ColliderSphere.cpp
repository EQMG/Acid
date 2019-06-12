#include "ColliderSphere.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "Scenes/Entity.hpp"

namespace acid
{
ColliderSphere::ColliderSphere(const float &radius, const Transform &localTransform) :
	Collider{localTransform, GizmoType::Create(Model::Create("Gizmos/Sphere.obj"), 3.0f, Colour::Blue)},
	m_shape{std::make_unique<btSphereShape>(radius)},
	m_radius{radius}
{
	m_localTransform.SetScale({m_radius, m_radius, m_radius});
}

ColliderSphere::~ColliderSphere()
{
}

void ColliderSphere::Start()
{
}

void ColliderSphere::Update()
{
	Collider::Update();
}

btCollisionShape *ColliderSphere::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderSphere::SetRadius(const float &radius)
{
	m_radius = radius;
	m_shape->setUnscaledRadius(m_radius);
	m_localTransform.SetScale({m_radius, m_radius, m_radius});
}

const Metadata &operator>>(const Metadata &metadata, ColliderSphere &collider)
{
	metadata.GetChild("Local Transform", collider.m_localTransform);
	metadata.GetChild("Radius", collider.m_radius);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const ColliderSphere &collider)
{
	metadata.SetChild("Local Transform", collider.m_localTransform);
	metadata.SetChild("Radius", collider.m_radius);
	return metadata;
}
}
