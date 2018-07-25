#include "ColliderCone.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCone::ColliderCone(const float &radius, const float &height) :
		m_shape(new btConeShape(radius, height)),
		m_radius(radius),
		m_height(height)
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ColliderCone::~ColliderCone()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ColliderCone::Start()
	{
	}

	void ColliderCone::Update()
	{
		m_shape->setRadius(m_radius);
		m_shape->setHeight(m_height);
	}

	void ColliderCone::Load(LoadedValue *value)
	{
		m_radius = value->GetChild("Radius")->Get<float>();
		m_height = value->GetChild("Height")->Get<float>();
	}

	void ColliderCone::Write(LoadedValue *destination)
	{
		destination->GetChild("Radius", true)->Set(m_radius);
		destination->GetChild("Height", true)->Set(m_height);
	}
}
