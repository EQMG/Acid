#include "ColliderSphere.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderSphere::ColliderSphere(const float &radius, const Transform &localTransform) :
		Collider(localTransform),
		m_shape(std::make_unique<btSphereShape>(radius)),
		m_radius(radius)
	{
	}

	ColliderSphere::~ColliderSphere()
	{
	}

	void ColliderSphere::Start()
	{
	}

	void ColliderSphere::Update()
	{
		m_shape->setUnscaledRadius(m_radius);
	}

	void ColliderSphere::Decode(const Metadata &metadata)
	{
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
		m_radius = metadata.GetChild<float>("Radius");
	}

	void ColliderSphere::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
		metadata.SetChild<float>("Radius", m_radius);
	}

	btCollisionShape *ColliderSphere::GetCollisionShape() const
	{
		return m_shape.get();
	}
}
