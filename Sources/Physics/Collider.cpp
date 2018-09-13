#include "Collider.hpp"

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "Objects/GameObject.hpp"

namespace acid
{
	Collider::Collider()
	{
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
