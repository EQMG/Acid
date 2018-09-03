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

	std::shared_ptr<btCollisionShape> ColliderCapsule::GetCollisionShape() const
	{
		return m_shape;
	}
}
