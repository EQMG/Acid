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

	void ColliderCone::Decode(const Node &node)
	{
		m_radius = node.GetChild<float>("Radius");
		m_height = node.GetChild<float>("Height");
	}

	void ColliderCone::Encode(Node &node) const
	{
		node.SetChild<float>("Radius", m_radius);
		node.SetChild<float>("Height", m_height);
	}

	std::shared_ptr<btCollisionShape> ColliderCone::GetCollisionShape() const
	{
		return m_shape;
	}
}
