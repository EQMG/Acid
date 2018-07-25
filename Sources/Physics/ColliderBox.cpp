#include "ColliderBox.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderBox::ColliderBox(const Vector3 &extents) :
		m_shape(new btBoxShape(ICollider::Convert(extents / 2.0f))),
		m_extents(extents)
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ColliderBox::~ColliderBox()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ColliderBox::Start()
	{
	}

	void ColliderBox::Update()
	{
	//	m_shape->setImplicitShapeDimensions(ICollider::Convert(m_extents)); // TODO
	}

	void ColliderBox::Load(LoadedValue *value)
	{
		m_extents = value->GetChild("Extents");
	}

	void ColliderBox::Write(LoadedValue *destination)
	{
		m_extents.Write(destination->GetChild("Extents", true));
	}
}
