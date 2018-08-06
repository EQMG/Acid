#include "ColliderBox.hpp"

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

	void ColliderBox::Load(LoadedValue *value)
	{
		m_extents = value->GetChild("Extents");
	}

	void ColliderBox::Write(LoadedValue *destination)
	{
		m_extents.Write(destination->GetChild("Extents", true));
	}

	btCollisionShape *ColliderBox::GetCollisionShape() const
	{
		return m_shape;
	}
}
