#include "ColliderCone.hpp"

#include <BulletCollision/CollisionShapes/btConeShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCone::ColliderCone(const float &radius, const float &height, const Transform &localTransform) :
		Collider(localTransform),
		m_shape(std::make_unique<btConeShape>(radius, height)),
		m_radius(radius),
		m_height(height)
	{
		auto gizmoTypeSquare = GizmoType::Resource(Model::Resource("Gizmos/Cone.obj"), 3.0f);
		m_gizmo = Gizmos::Get()->AddGizmo(new Gizmo(gizmoTypeSquare, localTransform, Colour::BLUE));
	}

	ColliderCone::~ColliderCone()
	{
		Gizmos::Get()->RemoveGizmo(m_gizmo);
	}

	void ColliderCone::Start()
	{
	}

	void ColliderCone::Update()
	{
		m_gizmo->SetTransform(GetGameObject()->GetTransform()); //  * m_localTransform
		m_shape->setRadius(m_radius);
		m_shape->setHeight(m_height);
	}

	void ColliderCone::Decode(const Metadata &metadata)
	{
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
		m_radius = metadata.GetChild<float>("Radius");
		m_height = metadata.GetChild<float>("Height");
	}

	void ColliderCone::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<float>("Height", m_height);
	}

	btCollisionShape *ColliderCone::GetCollisionShape() const
	{
		return m_shape.get();
	}
}
