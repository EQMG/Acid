#pragma once

#include "Collider.hpp"

class btCylinderShape;

namespace acid {
class ACID_EXPORT CylinderCollider : public Collider::Registrar<CylinderCollider> {
	inline static const bool Registered = Register("cylinder");
public:
	explicit CylinderCollider(float radius = 1.0f, float height = 1.0f, const Transform &localTransform = {});
	~CylinderCollider();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius);

	float GetHeight() const { return height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, CylinderCollider &collider);
	friend Node &operator<<(Node &node, const CylinderCollider &collider);

private:
	std::unique_ptr<btCylinderShape> shape;
	float radius;
	float height;
};
}
