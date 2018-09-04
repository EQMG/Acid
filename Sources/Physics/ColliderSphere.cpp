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

	void ColliderSphere::Decode(const Node &node)
	{
		m_radius = node.GetChild<float>("Radius");
	}

	void ColliderSphere::Encode(Node &node) const
	{
		node.SetChild<float>("Radius", m_radius);
	}

	btCollisionShape *ColliderSphere::GetCollisionShape() const
	{
		return m_shape;
	}
}
