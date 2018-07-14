#include "ShapeSphere.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeSphere::ShapeSphere(const float &radius) :
		m_shape(new btSphereShape(radius))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeSphere::~ShapeSphere()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeSphere::Load(LoadedValue *value)
	{
		float radius = value->GetChild("Radius")->Get<float>();
		m_shape->setUnscaledRadius(radius);
	}

	void ShapeSphere::Write(LoadedValue *destination)
	{
		float radius = m_shape->getRadius();
		destination->GetChild("Radius", true)->Set(radius);
	}
}
