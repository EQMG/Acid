#include "ColliderCylinder.hpp"

#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCylinder::ColliderCylinder(const float &radius, const float &height) :
		Collider(),
		m_shape(std::make_shared<btCylinderShape>(btVector3(radius, height / 2.0f, radius))),
		m_radius(radius),
		m_height(height)
	{
	}

	ColliderCylinder::~ColliderCylinder()
	{
	}

	void ColliderCylinder::Start()
	{
	}

	void ColliderCylinder::Update()
	{
	//	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height / 2.0f, m_radius)); // TODO
	}

	void ColliderCylinder::Decode(const Serialized &serialized)
	{
		m_radius = serialized.GetChild<float>("Radius");
		m_height = serialized.GetChild<float>("Height");
	}

	void ColliderCylinder::Encode(Serialized &serialized) const
	{
		serialized.SetChild<float>("Radius", m_radius);
		serialized.SetChild<float>("Height", m_height);
	}

	std::shared_ptr<btCollisionShape> ColliderCylinder::GetCollisionShape() const
	{
		return m_shape;
	}
}
