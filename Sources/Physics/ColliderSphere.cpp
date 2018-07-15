#include "ColliderSphere.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	ColliderSphere::ColliderSphere(const float &radius) :
		m_shape(new btSphereShape(radius)),
		m_radius(radius)
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ColliderSphere::~ColliderSphere()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ColliderSphere::Start()
	{
	}

	void ColliderSphere::Update()
	{
		m_shape->setUnscaledRadius(m_radius);
	}

	void ColliderSphere::Load(LoadedValue *value)
	{
		m_radius = value->GetChild("Radius")->Get<float>();
	}

	void ColliderSphere::Write(LoadedValue *destination)
	{
		destination->GetChild("Radius", true)->Set(m_radius);
	}
}
