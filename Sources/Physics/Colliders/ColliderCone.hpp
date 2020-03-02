#pragma once

#include "Collider.hpp"

class btConeShape;

namespace acid {
class ACID_EXPORT ColliderCone : public Collider::Registrar<ColliderCone> {
	inline static const bool Registered = Register("cone");
public:
	explicit ColliderCone(float radius = 1.0f, float height = 1.0f, const Transform &localTransform = {});
	~ColliderCone();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius);

	float GetHeight() const { return height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, ColliderCone &collider);
	friend Node &operator<<(Node &node, const ColliderCone &collider);

private:
	std::unique_ptr<btConeShape> shape;
	float radius;
	float height;
};
}
