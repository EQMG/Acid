#include "ShapeCapsule.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeCapsule::ShapeCapsule(const float &diameter, const float &height) :
		m_shape(new btCapsuleShape(diameter / 2.0f, height))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeCapsule::~ShapeCapsule()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeCapsule::Start()
	{
	}

	void ShapeCapsule::Update()
	{
	}

	void ShapeCapsule::Load(LoadedValue *value)
	{
		float diameter = value->GetChild("Diameter")->Get<float>();
		float height = value->GetChild("Height")->Get<float>();
		m_shape->setImplicitShapeDimensions(btVector3(diameter / 2.0f, 2.0f * height, diameter / 2.0f)); // TODO
	}

	void ShapeCapsule::Write(LoadedValue *destination)
	{
		float diameter = 2.0f * m_shape->getRadius();
		float height = 2.0f * m_shape->getHalfHeight();
		destination->GetChild("Diameter", true)->Set(diameter);
		destination->GetChild("Height", true)->Set(height);
	}

	float ShapeCapsule::GetDiameter() const
	{
		return 2.0f * m_shape->getRadius();
	}

	void ShapeCapsule::SetDiameter(const float &diameter)
	{
		m_shape->setImplicitShapeDimensions(btVector3(diameter / 2.0f, 2.0f * m_shape->getHalfHeight(), diameter / 2.0f)); // TODO
	}

	float ShapeCapsule::GetHeight() const
	{
		return 2.0f * m_shape->getHalfHeight();
	}

	void ShapeCapsule::SetHeight(const float &height)
	{
		m_shape->setImplicitShapeDimensions(btVector3(m_shape->getRadius(), height / 2.0f, m_shape->getRadius())); // TODO
	}
}
