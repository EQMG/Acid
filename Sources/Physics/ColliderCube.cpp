#include "ColliderCube.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCube::ColliderCube(const Vector3 &extents, const Transform &localTransform) :
		Collider(localTransform),
		m_shape(std::make_unique<btBoxShape>(Collider::Convert(extents / 2.0f))),
		m_extents(extents)
	{
		auto gizmoTypeSquare = GizmoType::Resource(Model::Resource("Gizmos/Cube.obj"), 3.0f);
		m_gizmo = Gizmos::Get()->AddGizmo(new Gizmo(gizmoTypeSquare, localTransform, Colour::RED));
	}

	ColliderCube::~ColliderCube()
	{
		Gizmos::Get()->RemoveGizmo(m_gizmo);
	}

	void ColliderCube::Start()
	{
	}

	void ColliderCube::Update()
	{
		m_gizmo->SetTransform(GetGameObject()->GetTransform()); //  * m_localTransform
	//	m_shape->setImplicitShapeDimensions(Collider::Convert(m_extents)); // TODO
	}

	void ColliderCube::Decode(const Metadata &metadata)
	{
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
		m_extents = metadata.GetChild<Vector3>("Extents");
	}

	void ColliderCube::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
		metadata.SetChild<Vector3>("Extents", m_extents);
	}

	btCollisionShape *ColliderCube::GetCollisionShape() const
	{
		return m_shape.get();
	}
}
