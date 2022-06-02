#pragma once

#include "Collider.hpp"

class btSphereShape;

namespace acid {
class ACID_PHYSICS_EXPORT SphereCollider : public Collider::Registrar<SphereCollider> {
	inline static const bool Registered = Register("sphere");
public:
	explicit SphereCollider(float radius = 0.5f, const Transform &localTransform = {});
	~SphereCollider();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius);

	friend const Node &operator>>(const Node &node, SphereCollider &collider);
	friend Node &operator<<(Node &node, const SphereCollider &collider);

private:
	std::unique_ptr<btSphereShape> shape;
	float radius;
};
}
