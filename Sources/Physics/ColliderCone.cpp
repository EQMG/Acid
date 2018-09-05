#include "ColliderCone.hpp"

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCone::ColliderCone(const float &radius, const float &height) :
		Collider(),
		m_shape(new btConeShape(radius, height)),
		m_radius(radius),
		m_height(height)
	{
	}

	ColliderCone::~ColliderCone()
	{
		delete m_shape;
	}

	void ColliderCone::Start()
	{
	}

	void ColliderCone::Update()
	{
		m_shape->setRadius(m_radius);
		m_shape->setHeight(m_height);
	}

	void ColliderCone::Decode(const Metadata &metadata)
	{
		m_radius = metadata.GetChild<float>("Radius");
		m_height = metadata.GetChild<float>("Height");
	}

	void ColliderCone::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<float>("Height", m_height);
	}

	btCollisionShape *ColliderCone::GetCollisionShape() const
	{
		return m_shape;
	}
}
