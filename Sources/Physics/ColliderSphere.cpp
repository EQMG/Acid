#include "ColliderSphere.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderSphere::ColliderSphere(const float &radius) :
		Collider(),
		m_shape(new btSphereShape(radius)),
		m_radius(radius)
	{
	}

	ColliderSphere::~ColliderSphere()
	{
		delete m_shape;
	}

	void ColliderSphere::Start()
	{
	}

	void ColliderSphere::Update()
	{
		m_shape->setUnscaledRadius(m_radius);
	}

	void ColliderSphere::Load(LoadedValue &value)
	{
		m_radius = value.GetChild("Radius")->Get<float>();
	}

	void ColliderSphere::Write(LoadedValue &destination)
	{
		destination.GetChild("Radius", true)->Set(m_radius);
	}

	btCollisionShape *ColliderSphere::GetCollisionShape() const
	{
		return m_shape;
	}
}
