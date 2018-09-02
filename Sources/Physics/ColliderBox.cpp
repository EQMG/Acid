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

	void ColliderBox::Load(LoadedValue &value)
	{
		m_extents = *value.FindChild("Extents");
	}

	void ColliderBox::Write(LoadedValue &destination)
	{
		m_extents.Write(*destination.FindChild("Extents", true));
	}

	std::shared_ptr<btCollisionShape> ColliderBox::GetCollisionShape() const
	{
		return m_shape;
	}
}
