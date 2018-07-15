#include "IShape.hpp"

#include "Objects/GameObject.hpp"

namespace fl
{
	IShape::IShape()
	{
	}

	IShape::~IShape()
	{
	}

	bool IShape::InFrustum(const Frustum &frustum)
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
		worldTransform.setOrigin(IShape::Convert(position));
		worldTransform.setRotation(IShape::Convert(rotation));

		btVector3 min = btVector3();
		btVector3 max = btVector3();
		shape->getAabb(worldTransform, min, max);

		return frustum.CubeInFrustum(Convert(min), Convert(max));
	}
}
