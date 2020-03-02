#pragma once

#include "Collider.hpp"

class btBoxShape;

namespace acid {
class ACID_EXPORT ColliderCube : public Collider::Registrar<ColliderCube> {
	inline static const bool Registered = Register("cube");
public:
	explicit ColliderCube(const Vector3f &extents = Vector3f(1.0f), const Transform &localTransform = {});
	~ColliderCube();

	btCollisionShape *GetCollisionShape() const override;

	const Vector3f &GetExtents() const { return extents; }
	void SetExtents(const Vector3f &extents);

	friend const Node &operator>>(const Node &node, ColliderCube &collider);
	friend Node &operator<<(Node &node, const ColliderCube &collider);

private:
	std::unique_ptr<btBoxShape> shape;
	Vector3f extents;
};
}
