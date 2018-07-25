#include "ColliderCapsule.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCapsule::ColliderCapsule(const float &radius, const float &height) :
		m_shape(new btCapsuleShape(radius, height)),
		m_radius(radius),
		m_height(height)
	{
		Scenes::Get()->GetCollisionShapes().push_back(m_shape);
	}

	ColliderCapsule::~ColliderCapsule()
	{
		Scenes::Get()->GetCollisionShapes().remove(m_shape);
		delete m_shape;
	}

	void ColliderCapsule::Start()
	{
	}

	void ColliderCapsule::Update()
	{
	//	m_shape->setImplicitShapeDimensions(btVector3(m_radius, m_height, m_radius)); // TODO
	}

	void ColliderCapsule::Load(LoadedValue *value)
	{
		m_radius = value->GetChild("Radius")->Get<float>();
		m_height = value->GetChild("Height")->Get<float>();
	}

	void ColliderCapsule::Write(LoadedValue *destination)
	{
		destination->GetChild("Radius", true)->Set(m_radius);
		destination->GetChild("Height", true)->Set(m_height);
	}
}
