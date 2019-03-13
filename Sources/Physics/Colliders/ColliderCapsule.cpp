#include "ColliderCapsule.hpp"

#include "Scenes/Scenes.hpp"
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <LinearMath/btAlignedObjectArray.h>

namespace acid
{
ColliderCapsule::ColliderCapsule(const float& radius, const float& height, const Transform& localTransform)
	: Collider(localTransform, GizmoType::Create(Model::Create("Gizmos/Capsule.obj"), 3.0f, Colour::Fuchsia)), m_shape(new btCapsuleShape(radius, height)), m_radius(radius), m_height(height)
{
	Scenes::Get()->GetPhysics()->GetCollisionShapes()->push_back(m_shape);
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}

ColliderCapsule::~ColliderCapsule()
{
	//	Scenes::Get()->GetPhysics()->GetCollisionShapes()->remove(m_shape);
}

void ColliderCapsule::Start() {}

void ColliderCapsule::Update()
{
	Collider::Update();
}

void ColliderCapsule::Decode(const Metadata& metadata)
{
	metadata.GetChild("Local Transform", m_localTransform);
	metadata.GetChild("Radius", m_radius);
	metadata.GetChild("Height", m_height);
}

void ColliderCapsule::Encode(Metadata& metadata) const
{
	metadata.SetChild("Local Transform", m_localTransform);
	metadata.SetChild("Radius", m_radius);
	metadata.SetChild("Height", m_height);
}

btCollisionShape* ColliderCapsule::GetCollisionShape() const
{
	return m_shape;
}

void ColliderCapsule::SetRadius(const float& radius)
{
	m_radius = radius;
	m_shape->setImplicitShapeDimensions(btVector3(m_radius, 0.5f * m_height, m_radius));
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}

void ColliderCapsule::SetHeight(const float& height)
{
	m_height = height;
	m_shape->setImplicitShapeDimensions(btVector3(m_radius, 0.5f * m_height, m_radius));
	m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
}
}
