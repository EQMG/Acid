#include "ColliderCone.hpp"

#include "Scenes/Scenes.hpp"
#include <BulletCollision/CollisionShapes/btConeShape.h>

namespace acid
{
ColliderCone::ColliderCone(const float& radius, const float& height, const Transform& localTransform)
	: Collider(localTransform, GizmoType::Create(Model::Create("Gizmos/Cone.obj"), 3.0f, Colour::Green)), m_shape(std::make_unique<btConeShape>(radius, height)), m_radius(radius), m_height(height)
{
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}

ColliderCone::~ColliderCone() {}

void ColliderCone::Start() {}

void ColliderCone::Update()
{
	Collider::Update();
}

void ColliderCone::Decode(const Metadata& metadata)
{
	metadata.GetChild("Local Transform", m_localTransform);
	metadata.GetChild("Radius", m_radius);
	metadata.GetChild("Height", m_height);
}

void ColliderCone::Encode(Metadata& metadata) const
{
	metadata.SetChild("Local Transform", m_localTransform);
	metadata.SetChild("Radius", m_radius);
	metadata.SetChild("Height", m_height);
}

btCollisionShape* ColliderCone::GetCollisionShape() const
{
	return m_shape.get();
}

void ColliderCone::SetRadius(const float& radius)
{
	m_radius = radius;
	m_shape->setRadius(m_radius);
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}

void ColliderCone::SetHeight(const float& height)
{
	m_height = height;
	m_shape->setHeight(m_height);
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}
}
