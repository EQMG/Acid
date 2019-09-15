#pragma once

#include "Collider.hpp"

class btBoxShape;

namespace acid {
class ACID_EXPORT
ColliderCube
:
public
Collider
{
public:
	explicit ColliderCube(const Vector3f &extents = {1.0f}, const Transform &localTransform = {});

	~ColliderCube();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	const Vector3f &GetExtents() const { return m_extents; }

	void SetExtents(const Vector3f &extents);

	friend const Node &operator>>(const Node &node, ColliderCube &collider);

	friend Node &operator<<(Node &node, const ColliderCube &collider);

private:
	std::unique_ptr<btBoxShape> m_shape;
	Vector3f m_extents;
};
}
