#pragma once

#include "Collider.hpp"

class btCapsuleShape;

namespace acid {
class ACID_EXPORT ColliderCapsule : public Collider::Registrar<ColliderCapsule> {
	inline static const bool Registered = Register("capsule");
public:
	explicit ColliderCapsule(float radius = 0.5f, float height = 1.0f, const Transform &localTransform = {});
	~ColliderCapsule();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius);

	float GetHeight() const { return height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, ColliderCapsule &collider);
	friend Node &operator<<(Node &node, const ColliderCapsule &collider);

private:
	std::unique_ptr<btCapsuleShape> shape;
	float radius;
	float height;
};
}
