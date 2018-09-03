#include "ColliderCone.hpp"

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCone::ColliderCone(const float &radius, const float &height) :
		Collider(),
		m_shape(std::make_shared<btConeShape>(radius, height)),
		m_radius(radius),
		m_height(height)
	{
	}

	ColliderCone::~ColliderCone()
	{
	}

	void ColliderCone::Start()
	{
	}

	void ColliderCone::Update()
	{
		m_shape->setRadius(m_radius);
		m_shape->setHeight(m_height);
	}

	void ColliderCone::Decode(const Serialized &serialized)
	{
		m_radius = serialized.GetChild<float>("Radius");
		m_height = serialized.GetChild<float>("Height");
	}

	void ColliderCone::Encode(Serialized &serialized) const
	{
		serialized.SetChild<float>("Radius", m_radius);
		serialized.SetChild<float>("Height", m_height);
	}

	std::shared_ptr<btCollisionShape> ColliderCone::GetCollisionShape() const
	{
		return m_shape;
	}
}
