#include "ShapeCone.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeCone::ShapeCone(const float &diameter, const float &height) :
		m_shape(new btConeShape(diameter / 2.0f, height))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeCone::~ShapeCone()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeCone::Start()
	{
	}

	void ShapeCone::Update()
	{
	}

	void ShapeCone::Load(LoadedValue *value)
	{
		float diameter = value->GetChild("Diameter")->Get<float>();
		float height = value->GetChild("Height")->Get<float>();
		m_shape->setRadius(diameter / 2.0f);
		m_shape->setHeight(height);
	}

	void ShapeCone::Write(LoadedValue *destination)
	{
		float diameter = 2.0f * m_shape->getRadius();
		float height = m_shape->getHeight();
		destination->GetChild("Diameter", true)->Set(diameter);
		destination->GetChild("Height", true)->Set(height);
	}

	float ShapeCone::GetDiameter() const
	{
		return 2.0f * m_shape->getRadius();
	}

	void ShapeCone::SetDiameter(const float &diameter)
	{
		m_shape->setRadius(diameter);
	}

	float ShapeCone::GetHeight() const
	{
		return m_shape->getHeight();
	}

	void ShapeCone::SetHeight(const float &height)
	{
		m_shape->setHeight(height);
	}
}
