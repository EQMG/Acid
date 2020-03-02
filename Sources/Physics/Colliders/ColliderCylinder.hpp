#pragma once

#include "Collider.hpp"

class btCylinderShape;

namespace acid {
class ACID_EXPORT ColliderCylinder : public Collider::Registrar<ColliderCylinder> {
	inline static const bool Registered = Register("cylinder");
public:
	explicit ColliderCylinder(float radius = 1.0f, float height = 1.0f, const Transform &localTransform = {});
	~ColliderCylinder();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius);

	float GetHeight() const { return height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, ColliderCylinder &collider);
	friend Node &operator<<(Node &node, const ColliderCylinder &collider);

private:
	std::unique_ptr<btCylinderShape> shape;
	float radius;
	float height;
};
}
