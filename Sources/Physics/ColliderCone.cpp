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

	void ColliderCone::Load(LoadedValue *value)
	{
		m_radius = value->GetChild("Radius")->Get<float>();
		m_height = value->GetChild("Height")->Get<float>();
	}

	void ColliderCone::Write(LoadedValue *destination)
	{
		destination->GetChild("Radius", true)->Set(m_radius);
		destination->GetChild("Height", true)->Set(m_height);
	}

	btCollisionShape *ColliderCone::GetCollisionShape() const
	{
		return m_shape;
	}
}
