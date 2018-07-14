#include "ShapeCapsule.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeCapsule::ShapeCapsule(const float &radius, const float &halfHeight) :
		m_shape(new btCapsuleShape(radius, 2.0f * halfHeight))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeCapsule::~ShapeCapsule()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeCapsule::Load(LoadedValue *value)
	{
		float radius = value->GetChild("Radius")->Get<float>();
		float halfHeight = value->GetChild("Half Height")->Get<float>();
		m_shape->setImplicitShapeDimensions(btVector3(radius, halfHeight, radius)); // TODO
	}

	void ShapeCapsule::Write(LoadedValue *destination)
	{
		float radius = m_shape->getRadius();
		float halfHeight = m_shape->getHalfHeight();
		destination->GetChild("Radius", true)->Set(radius);
		destination->GetChild("Half Height", true)->Set(halfHeight);
	}
}
