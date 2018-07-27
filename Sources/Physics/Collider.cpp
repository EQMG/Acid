#include "Collider.hpp"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include "Objects/GameObject.hpp"

namespace acid
{
	Collider::Collider()
	{
	}

	Collider::~Collider()
	{
	}

	bool Collider::InFrustum(const Frustum &frustum)
	{
		btCollisionShape *shape = GetCollisionShape();

		if (shape == nullptr)
		{
			return true;
		}

		Vector3 position = GetGameObject()->GetTransform().GetPosition();
		Quaternion rotation = GetGameObject()->GetTransform().GetRotation();

		btTransform worldTransform = btTransform();
		worldTransform.setIdentity();
		worldTransform.setOrigin(Collider::Convert(position));
		worldTransform.setRotation(Collider::Convert(rotation));

		btVector3 min = btVector3();
		btVector3 max = btVector3();
		shape->getAabb(worldTransform, min, max);

		return frustum.CubeInFrustum(Convert(min), Convert(max));
	}

	btVector3 Collider::Convert(const Vector3 &vector)
	{
		return btVector3(vector.m_x, vector.m_y, vector.m_z);
	}

	Vector3 Collider::Convert(const btVector3 &vector)
	{
		return Vector3(vector.getX(), vector.getY(), vector.getZ());
	}

	btQuaternion Collider::Convert(const Quaternion &quaternion)
	{
		return btQuaternion(quaternion.m_x, quaternion.m_y, quaternion.m_z, quaternion.m_w);
	}

	Quaternion Collider::Convert(const btQuaternion &quaternion)
	{
		return Quaternion(quaternion.getX(), quaternion.getY(), quaternion.getZ(), quaternion.getW());
	}
}
