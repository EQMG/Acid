#include "ColliderBox.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderBox::ColliderBox(const Vector3 &extents) :
		Collider(),
		m_shape(std::make_shared<btBoxShape>(Collider::Convert(extents / 2.0f))),
		m_extents(extents)
	{
	}

	ColliderBox::~ColliderBox()
	{
	}

	void ColliderBox::Start()
	{
	}

	void ColliderBox::Update()
	{
	//	m_shape->setImplicitShapeDimensions(Collider::Convert(m_extents)); // TODO
	}

	void ColliderBox::Decode(const Serialized &serialized)
	{
		m_extents = serialized.GetChild<Vector3>("Extents");
	}

	void ColliderBox::Encode(Serialized &serialized) const
	{
		serialized.SetChild<Vector3>("Extents", m_extents);
	}

	std::shared_ptr<btCollisionShape> ColliderBox::GetCollisionShape() const
	{
		return m_shape;
	}
}
