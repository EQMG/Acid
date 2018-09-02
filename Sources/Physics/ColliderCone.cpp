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

	void ColliderCone::Load(LoadedValue &value)
	{
		m_radius = value.FindChild("Radius")->Get<float>();
		m_height = value.FindChild("Height")->Get<float>();
	}

	void ColliderCone::Write(LoadedValue &destination)
	{
		destination.FindChild("Radius", true)->Set(m_radius);
		destination.FindChild("Height", true)->Set(m_height);
	}

	std::shared_ptr<btCollisionShape> ColliderCone::GetCollisionShape() const
	{
		return m_shape;
	}
}
