#include "ShapeCylinder.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeCylinder::ShapeCylinder(const float &diameter, const float &height) :
		m_shape(new btCylinderShape(btVector3(diameter / 2.0f, height / 2.0f, diameter / 2.0f)))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeCylinder::~ShapeCylinder()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeCylinder::Start()
	{
	}

	void ShapeCylinder::Update()
	{
	}

	void ShapeCylinder::Load(LoadedValue *value)
	{
		float diameter = value->GetChild("Diameter")->Get<float>();
		float height = value->GetChild("Height")->Get<float>();
		m_shape->setImplicitShapeDimensions(btVector3(diameter / 2.0f, height / 2.0f, diameter / 2.0f)); // TODO
	}

	void ShapeCylinder::Write(LoadedValue *destination)
	{
		float diameter = 2.0f * m_shape->getRadius();
		float height = m_shape->getHalfExtentsWithoutMargin().getY();
		destination->GetChild("Diameter", true)->Set(diameter);
		destination->GetChild("Height", true)->Set(height);
	}

	float ShapeCylinder::GetDiameter() const
	{
		return 2.0f * m_shape->getRadius();
	}

	void ShapeCylinder::SetDiameter(const float &diameter)
	{
		m_shape->setImplicitShapeDimensions(btVector3(diameter / 2.0f, m_shape->getHalfExtentsWithoutMargin().getY(), diameter / 2.0f)); // TODO
	}

	float ShapeCylinder::GetHeight() const
	{
		return 2.0f * m_shape->getHalfExtentsWithoutMargin().getY();
	}

	void ShapeCylinder::SetHeight(const float &height)
	{
		m_shape->setImplicitShapeDimensions(btVector3(m_shape->getRadius(), height / 2.0f, m_shape->getRadius())); // TODO
	}
}
