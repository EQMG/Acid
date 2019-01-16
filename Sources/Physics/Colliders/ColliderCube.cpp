#include "ColliderCube.hpp"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Scenes/Scenes.hpp"

namespace acid
{
	ColliderCube::ColliderCube(const Vector3 &extents, const Transform &localTransform) :
		Collider(localTransform, GizmoType::Create(Model::Create("Gizmos/Cube.obj"), 3.0f, Colour::Red)),
		m_shape(std::make_unique<btBoxShape>(Collider::Convert(extents / 2.0f))),
		m_extents(extents)
	{
		m_localTransform.SetScaling(m_extents);
	}

	ColliderCube::~ColliderCube()
	{
	}

	void ColliderCube::Start()
	{
	}

	void ColliderCube::Update()
	{
		Collider::Update();
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

	void ColliderCube::SetExtents(const Vector3 &extents)
	{
		m_extents = extents;
		m_shape->setImplicitShapeDimensions(Collider::Convert(m_extents));
		m_localTransform.SetScaling(m_extents);
	}
}
