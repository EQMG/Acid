#include "ColliderCone.hpp"

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCone::ColliderCone(const float &radius, const float &height, const Transform &localTransform) :
		Collider(localTransform, GizmoType::Resource(Model::Resource("Gizmos/Cone.obj"), 3.0f, Colour::GREEN)),
		m_shape(std::make_unique<btConeShape>(radius, height)),
		m_radius(radius),
		m_height(height)
	{
		m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
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

	void ColliderCone::Decode(const Metadata &metadata)
	{
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
		m_radius = metadata.GetChild<float>("Radius");
		m_height = metadata.GetChild<float>("Height");
	}

	void ColliderCone::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<float>("Height", m_height);
	}

	btCollisionShape *ColliderCone::GetCollisionShape() const
	{
		return m_shape.get();
	}

	void ColliderCone::SetRadius(const float &radius)
	{
		m_radius = radius;
		m_shape->setRadius(m_radius);
		m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
	}

	void ColliderCone::SetHeight(const float &height)
	{
		m_height = height;
		m_shape->setHeight(m_height);
		m_localTransform.SetScaling(Vector3(m_radius, m_height, m_radius));
	}
}
