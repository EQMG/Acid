#pragma once

#include "Collider.hpp"

class btBoxShape;

namespace acid
{
class ACID_EXPORT ColliderCube :
	public Collider
{
public:
	explicit ColliderCube(const Vector3f &extents = Vector3f::One, const Transform &localTransform = Transform::Identity);

	~ColliderCube();

	void Start() override;

	void Update() override;

	void Decode(const Metadata &metadata) override;

	void Encode(Metadata &metadata) const override;

	btCollisionShape *GetCollisionShape() const override;

	const Vector3f &GetExtents() const { return m_extents; }

	void SetExtents(const Vector3f &extents);

private:
	std::unique_ptr<btBoxShape> m_shape;
	Vector3f m_extents;
};
}
