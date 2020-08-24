#pragma once

#include "Collider.hpp"

class btBoxShape;

namespace acid {
class ACID_EXPORT CubeCollider : public Collider::Registrar<CubeCollider> {
	static const bool Registered;
public:
	explicit CubeCollider(const Vector3f &extents = Vector3f(1.0f), const Transform &localTransform = {});
	~CubeCollider();

	btCollisionShape *GetCollisionShape() const override;

	const Vector3f &GetExtents() const { return extents; }
	void SetExtents(const Vector3f &extents);

	friend const Node &operator>>(const Node &node, CubeCollider &collider);
	friend Node &operator<<(Node &node, const CubeCollider &collider);

private:
	std::unique_ptr<btBoxShape> shape;
	Vector3f extents;
};
}
