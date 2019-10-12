#pragma once

#include "Collider.hpp"

class btBoxShape;

namespace acid {
class ACID_EXPORT ColliderCube : public Collider::Registrar<ColliderCube> {
public:
	explicit ColliderCube(const Vector3f &extents = Vector3f(1.0f), const Transform &localTransform = {});

	~ColliderCube();

	btCollisionShape *GetCollisionShape() const override;

	const Vector3f &GetExtents() const { return m_extents; }
	void SetExtents(const Vector3f &extents);

	friend const Node &operator>>(const Node &node, ColliderCube &collider);
	friend Node &operator<<(Node &node, const ColliderCube &collider);

private:
	static bool registered;

	std::unique_ptr<btBoxShape> m_shape;
	Vector3f m_extents;
};
}
