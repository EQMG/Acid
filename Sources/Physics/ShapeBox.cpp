#include "ShapeBox.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeBox::ShapeBox(const Vector3 &extents) :
		m_shape(new btBoxShape(IShape::Convert(extents / 2.0f)))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeBox::~ShapeBox()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeBox::Start()
	{
	}

	void ShapeBox::Update()
	{
	}

	void ShapeBox::Load(LoadedValue *value)
	{
		Vector3 extents = Vector3();
		extents = value->GetChild("Extents");
		m_shape->setLocalScaling(IShape::Convert(extents / 2.0f));
	}

	void ShapeBox::Write(LoadedValue *destination)
	{
		btVector3 halfExtents = m_shape->getHalfExtentsWithoutMargin();
		IShape::Convert(2.0f * halfExtents).Write(destination->GetChild("Extents", true));
	}

	Vector3 ShapeBox::GetExtents() const
	{
		return IShape::Convert(2.0f * m_shape->getHalfExtentsWithoutMargin());
	}

	void ShapeBox::SetExtents(const Vector3 &extents)
	{
		m_shape->setLocalScaling(IShape::Convert(extents / 2.0f)); // TODO
	}
}
