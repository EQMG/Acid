#include "ShapeCylinder.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeCylinder::ShapeCylinder(const Vector3 &halfExtents) :
		m_shape(new btCylinderShape(btVector3(halfExtents.m_x, halfExtents.m_y, halfExtents.m_z)))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeCylinder::~ShapeCylinder()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeCylinder::Load(LoadedValue *value)
	{
		Vector3 halfExtents = Vector3();
		halfExtents = value->GetChild("Half Extents");
		m_shape->setLocalScaling(btVector3(halfExtents.m_x, halfExtents.m_y, halfExtents.m_z)); // TODO
	}

	void ShapeCylinder::Write(LoadedValue *destination)
	{
		btVector3 halfExtents = m_shape->getHalfExtentsWithoutMargin();
		Vector3(halfExtents.getX(), halfExtents.getY(), halfExtents.getZ()).Write(destination->GetChild("Half Extents", true));
	}
}
