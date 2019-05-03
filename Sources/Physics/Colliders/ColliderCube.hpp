#pragma once

#include "Collider.hpp"

class btBoxShape;

namespace acid
{
class ACID_EXPORT ColliderCube :
	public Collider
{
public:
	explicit ColliderCube(const Vector3f &extents = Vector3f::One, const Transform &localTransform = Transform::Zero);

	~ColliderCube();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	const Vector3f &GetExtents() const { return m_extents; }

	void SetExtents(const Vector3f &extents);

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, ColliderCube &collider);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const ColliderCube &collider);

private:
	std::unique_ptr<btBoxShape> m_shape;
	Vector3f m_extents;
};
}
