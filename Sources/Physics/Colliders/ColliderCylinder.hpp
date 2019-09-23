#pragma once

#include "Collider.hpp"

class btCylinderShape;

namespace acid {
class ACID_EXPORT ColliderCylinder : public Collider {
public:
	explicit ColliderCylinder(float radius = 1.0f, float height = 1.0f, const Transform &localTransform = {});

	~ColliderCylinder();

	void Start() override;
	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return m_radius; }
	void SetRadius(float radius);

	float GetHeight() const { return m_height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, ColliderCylinder &collider);
	friend Node &operator<<(Node &node, const ColliderCylinder &collider);

private:
	static Registrar<ColliderCylinder> registered;

	std::unique_ptr<btCylinderShape> m_shape;
	float m_radius;
	float m_height;
};
}
