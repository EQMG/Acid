#include "ShapeSphere.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ShapeSphere::ShapeSphere(const float &diameter) :
		m_shape(new btSphereShape(diameter / 2.0f))
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ShapeSphere::~ShapeSphere()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ShapeSphere::Start()
	{
	}

	void ShapeSphere::Update()
	{
	}

	void ShapeSphere::Load(LoadedValue *value)
	{
		float diameter = value->GetChild("Diameter")->Get<float>();
		m_shape->setUnscaledRadius(diameter / 2.0f);
	}

	void ShapeSphere::Write(LoadedValue *destination)
	{
		float diameter = 2.0f * m_shape->getRadius();
		destination->GetChild("Diameter", true)->Set(diameter);
	}

	float ShapeSphere::GetDiameter() const
	{
		return 2.0f * m_shape->getRadius();
	}

	void ShapeSphere::SetDiameter(const float &diameter)
	{
		m_shape->setUnscaledRadius(diameter / 2.0f);
	}

}
