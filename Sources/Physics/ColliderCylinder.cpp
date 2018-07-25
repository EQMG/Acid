#include "ColliderCylinder.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCylinder::ColliderCylinder(const float &radius, const float &height) :
		m_shape(new btCylinderShape(btVector3(radius, height / 2.0f, radius))),
		m_radius(radius),
		m_height(height)
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ColliderCylinder::~ColliderCylinder()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ColliderCylinder::Start()
	{
	}

	void ColliderCylinder::Update()
	{
	//	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height / 2.0f, m_radius)); // TODO
	}

	void ColliderCylinder::Load(LoadedValue *value)
	{
		m_radius = value->GetChild("Radius")->Get<float>();
		m_height = value->GetChild("Height")->Get<float>();
	}

	void ColliderCylinder::Write(LoadedValue *destination)
	{
		destination->GetChild("Radius", true)->Set(m_radius);
		destination->GetChild("Height", true)->Set(m_height);
	}
}
