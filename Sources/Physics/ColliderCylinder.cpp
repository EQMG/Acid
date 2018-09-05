#include "ColliderCylinder.hpp"

#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCylinder::ColliderCylinder(const float &radius, const float &height) :
		Collider(),
		m_shape(new btCylinderShape(btVector3(radius, height / 2.0f, radius))),
		m_radius(radius),
		m_height(height)
	{
	}

	ColliderCylinder::~ColliderCylinder()
	{
		delete m_shape;
	}

	void ColliderCylinder::Start()
	{
	}

	void ColliderCylinder::Update()
	{
	//	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height / 2.0f, m_radius)); // TODO
	}

	void ColliderCylinder::Decode(const Metadata &metadata)
	{
		m_radius = metadata.GetChild<float>("Radius");
		m_height = metadata.GetChild<float>("Height");
	}

	void ColliderCylinder::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<float>("Height", m_height);
	}

	btCollisionShape *ColliderCylinder::GetCollisionShape() const
	{
		return m_shape;
	}
}
