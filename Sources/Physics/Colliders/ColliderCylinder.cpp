#include "ColliderCylinder.hpp"

#include "Scenes/Scenes.hpp"
#include <BulletCollision/CollisionShapes/btCylinderShape.h>

namespace acid
{
ColliderCylinder::ColliderCylinder(const float& radius, const float& height, const Transform& localTransform)
	: Collider(localTransform, GizmoType::Create(Model::Create("Gizmos/Cylinder.obj"), 3.0f, Colour::Yellow)), m_shape(std::make_unique<btCylinderShape>(btVector3(radius, height / 2.0f, radius))), m_radius(radius), m_height(height)
{
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}

ColliderCylinder::~ColliderCylinder() {}

void ColliderCylinder::Start() {}

void ColliderCylinder::Update()
{
	Collider::Update();
}

void ColliderCylinder::Decode(const Metadata& metadata)
{
	metadata.GetChild("Local Transform", m_localTransform);
	metadata.GetChild("Radius", m_radius);
	metadata.GetChild("Height", m_height);
}

void ColliderCylinder::Encode(Metadata& metadata) const
{
	metadata.SetChild("Local Transform", m_localTransform);
	metadata.SetChild("Radius", m_radius);
	metadata.SetChild("Height", m_height);
}

btCollisionShape* ColliderCylinder::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderCylinder::SetRadius(const float& radius)
{
	m_radius = radius;
	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height / 2.0f, m_radius));
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}

void ColliderCylinder::SetHeight(const float& height)
{
	m_height = height;
	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height / 2.0f, m_radius));
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}
}
