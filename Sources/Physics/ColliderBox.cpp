#include "ColliderBox.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderBox::ColliderBox(const Vector3 &extents) :
		Collider(),
		m_shape(new btBoxShape(Collider::Convert(extents / 2.0f))),
		m_extents(extents)
	{
	}

	ColliderBox::~ColliderBox()
	{
		delete m_shape;
	}

	void ColliderBox::Start()
	{
	}

	void ColliderBox::Update()
	{
	//	m_shape->setImplicitShapeDimensions(Collider::Convert(m_extents)); // TODO
	}

	void ColliderBox::Decode(const Node &node)
	{
		m_extents = node.GetChild<Vector3>("Extents");
	}

	void ColliderBox::Encode(Node &node) const
	{
		node.SetChild<Vector3>("Extents", m_extents);
	}

	btCollisionShape *ColliderBox::GetCollisionShape() const
	{
		return m_shape;
	}
}
