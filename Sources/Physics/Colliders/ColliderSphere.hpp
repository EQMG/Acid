#pragma once

#include "Collider.hpp"

class btSphereShape;

namespace acid {
class ACID_EXPORT ColliderSphere : public Collider::Registrar<ColliderSphere> {
public:
	explicit ColliderSphere(float radius = 0.5f, const Transform &localTransform = {});

	~ColliderSphere();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return m_radius; }
	void SetRadius(float radius);

	friend const Node &operator>>(const Node &node, ColliderSphere &collider);
	friend Node &operator<<(Node &node, const ColliderSphere &collider);

private:
	static bool registered;

	std::unique_ptr<btSphereShape> m_shape;
	float m_radius;
};
}
