#pragma once

#include "Collider.hpp"

class btConeShape;

namespace acid {
class ACID_EXPORT ConeCollider : public Collider::Registrar<ConeCollider> {
	static const bool Registered;
public:
	explicit ConeCollider(float radius = 1.0f, float height = 1.0f, const Transform &localTransform = {});
	~ConeCollider();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius);

	float GetHeight() const { return height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, ConeCollider &collider);
	friend Node &operator<<(Node &node, const ConeCollider &collider);

private:
	std::unique_ptr<btConeShape> shape;
	float radius;
	float height;
};
}
