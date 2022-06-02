#pragma once

#include "Collider.hpp"

class btCapsuleShape;

namespace acid {
class ACID_PHYSICS_EXPORT CapsuleCollider : public Collider::Registrar<CapsuleCollider> {
	inline static const bool Registered = Register("capsule");
public:
	explicit CapsuleCollider(float radius = 0.5f, float height = 1.0f, const Transform &localTransform = {});
	~CapsuleCollider();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius);

	float GetHeight() const { return height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, CapsuleCollider &collider);
	friend Node &operator<<(Node &node, const CapsuleCollider &collider);

private:
	std::unique_ptr<btCapsuleShape> shape;
	float radius;
	float height;
};
}
