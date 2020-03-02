#pragma once

#include "Collider.hpp"

class btSphereShape;

namespace acid {
class ACID_EXPORT ColliderSphere : public Collider::Registrar<ColliderSphere> {
	inline static const bool Registered = Register("sphere");
public:
	explicit ColliderSphere(float radius = 0.5f, const Transform &localTransform = {});
	~ColliderSphere();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius);

	friend const Node &operator>>(const Node &node, ColliderSphere &collider);
	friend Node &operator<<(Node &node, const ColliderSphere &collider);

private:
	std::unique_ptr<btSphereShape> shape;
	float radius;
};
}
