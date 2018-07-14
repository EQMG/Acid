#include "ShapeCone.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeCone::ShapeCone(const float &radius, const float &height) :
		m_shape(new btConeShape(radius, height))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeCone::~ShapeCone()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeCone::Load(LoadedValue *value)
	{
		float radius = value->GetChild("Radius")->Get<float>();
		float height = value->GetChild("Height")->Get<float>();
		m_shape->setRadius(radius);
		m_shape->setHeight(height);
	}

	void ShapeCone::Write(LoadedValue *destination)
	{
		float radius = m_shape->getRadius();
		float height = m_shape->getHeight();
		destination->GetChild("Radius", true)->Set(radius);
		destination->GetChild("Height", true)->Set(height);
	}
}
