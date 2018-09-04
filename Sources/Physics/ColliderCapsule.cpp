#include "ColliderCapsule.hpp"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCapsule::ColliderCapsule(const float &radius, const float &height) :
		Collider(),
		m_shape(new btCapsuleShape(radius, height)),
		m_radius(radius),
		m_height(height)
	{
	}

	ColliderCapsule::~ColliderCapsule()
	{
		delete m_shape;
	}

	void ColliderCapsule::Start()
	{
	}

	void ColliderCapsule::Update()
	{
	//	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height, m_radius)); // TODO
	}

	void ColliderCapsule::Decode(const Node &node)
	{
		m_radius = node.GetChild<float>("Radius");
		m_height = node.GetChild<float>("Height");
	}

	void ColliderCapsule::Encode(Node &node) const
	{
		node.SetChild<float>("Radius", m_radius);
		node.SetChild<float>("Height", m_height);
	}

	btCollisionShape *ColliderCapsule::GetCollisionShape() const
	{
		return m_shape;
	}
}
