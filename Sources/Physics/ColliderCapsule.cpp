#include "ColliderCapsule.hpp"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCapsule::ColliderCapsule(const float &radius, const float &height) :
		Collider(),
		m_shape(std::make_shared<btCapsuleShape>(radius, height)),
		m_radius(radius),
		m_height(height)
	{
	}

	ColliderCapsule::~ColliderCapsule()
	{
	}

	void ColliderCapsule::Start()
	{
	}

	void ColliderCapsule::Update()
	{
		//	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height, m_radius)); // TODO
	}

	void ColliderCapsule::Load(LoadedValue &value)
	{
		m_radius = value.FindChild("Radius")->Get<float>();
		m_height = value.FindChild("Height")->Get<float>();
	}

	void ColliderCapsule::Write(LoadedValue &destination)
	{
		destination.FindChild("Radius", true)->Set(m_radius);
		destination.FindChild("Height", true)->Set(m_height);
	}

	std::shared_ptr<btCollisionShape> ColliderCapsule::GetCollisionShape() const
	{
		return m_shape;
	}
}
