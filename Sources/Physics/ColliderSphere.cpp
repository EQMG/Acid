#include "ColliderSphere.hpp"

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderSphere::ColliderSphere(const float &radius) :
		Collider(),
		m_shape(std::make_shared<btSphereShape>(radius)),
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

	void ColliderSphere::Decode(const Node &node)
	{
		m_radius = node.GetChild<float>("Radius");
	}

	void ColliderSphere::Encode(Node &node) const
	{
		node.SetChild<float>("Radius", m_radius);
	}

	std::shared_ptr<btCollisionShape> ColliderSphere::GetCollisionShape() const
	{
		return m_shape;
	}
}
