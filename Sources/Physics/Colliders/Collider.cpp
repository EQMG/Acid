#include "Collider.hpp"

#include "Maths/Maths.hpp"
#include "Physics/CollisionObject.hpp"
#include "Scenes/Entity.hpp"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

namespace acid
{
Collider::Collider(const Transform& localTransform, const std::shared_ptr<GizmoType>& gizmoType) : m_localTransform(localTransform), m_gizmo(nullptr)
{
#if defined(ACID_VERBOSE)
	if(gizmoType != nullptr)
		{
			m_gizmo = Gizmos::Get()->AddGizmo(new Gizmo(gizmoType, localTransform));
		}
#endif
}

Collider::~Collider()
{
	Gizmos::Get()->RemoveGizmo(m_gizmo);
}

void Collider::Update()
{
	if(m_gizmo != nullptr)
		{
			m_gizmo->SetTransform(GetParent()->GetWorldTransform() * m_localTransform);
		}
}

void Collider::SetLocalTransform(const Transform& localTransform)
{
	m_localTransform = localTransform;

	auto collisionObject = GetParent()->GetComponent<CollisionObject>();

	if(collisionObject != nullptr)
		{
			collisionObject->SetChildTransform(this, m_localTransform);
		}
}

btVector3 Collider::Convert(const Vector3& vector)
{
	return btVector3(vector.m_x, vector.m_y, vector.m_z);
}

Vector3 Collider::Convert(const btVector3& vector)
{
	return Vector3(vector.getX(), vector.getY(), vector.getZ());
}

btQuaternion Collider::Convert(const Quaternion& quaternion)
{
	return btQuaternion(quaternion.m_x, quaternion.m_y, quaternion.m_z, quaternion.m_w);
}

Quaternion Collider::Convert(const btQuaternion& quaternion)
{
	return Quaternion(quaternion.getX(), quaternion.getY(), quaternion.getZ(), quaternion.getW());
}

btTransform Collider::Convert(const Transform& transform)
{
	btQuaternion rotation = btQuaternion();
	rotation.setEulerZYX(transform.GetRotation().m_y * Maths::DegToRad, transform.GetRotation().m_x * Maths::DegToRad, transform.GetRotation().m_z * Maths::DegToRad);

	btTransform worldTransform = btTransform();
	worldTransform.setIdentity();
	worldTransform.setOrigin(Convert(transform.GetPosition()));
	worldTransform.setRotation(rotation);
	return worldTransform;
}

Transform Collider::Convert(const btTransform& transform, const Vector3& scaling)
{
	btVector3 position = transform.getOrigin();
	float yaw, pitch, roll;
	transform.getBasis().getEulerYPR(yaw, pitch, roll);
	return Transform(Convert(position), Vector3(pitch, yaw, roll) * Maths::RadToDeg, scaling);
}
}
