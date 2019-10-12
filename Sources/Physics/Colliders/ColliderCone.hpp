#pragma once

#include "Collider.hpp"

class btConeShape;

namespace acid {
class ACID_EXPORT ColliderCone : public Collider::Registrar<ColliderCone> {
public:
	explicit ColliderCone(float radius = 1.0f, float height = 1.0f, const Transform &localTransform = {});

	~ColliderCone();

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return m_radius; }
	void SetRadius(float radius);

	float GetHeight() const { return m_height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, ColliderCone &collider);
	friend Node &operator<<(Node &node, const ColliderCone &collider);

private:
	static bool registered;

	std::unique_ptr<btConeShape> m_shape;
	float m_radius;
	float m_height;
};
}
