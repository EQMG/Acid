#pragma once

#include "Collider.hpp"

class btSphereShape;

namespace acid {
class ACID_EXPORT ColliderSphere : public Collider {
public:
	explicit ColliderSphere(float radius = 0.5f, const Transform & localTransform = {});

	~ColliderSphere();

	void Start() override;
	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return m_radius; }
	void SetRadius(float radius);

	friend const Node &operator>>(const Node &node, ColliderSphere &collider);
	friend Node &operator<<(Node &node, const ColliderSphere &collider);

private:
	std::unique_ptr<btSphereShape> m_shape;
	float m_radius;
};
}
