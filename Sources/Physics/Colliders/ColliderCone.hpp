#pragma once

#include "Collider.hpp"

class btConeShape;

namespace acid {
class ACID_EXPORT
ColliderCone
:
public
Collider
{
public:
	explicit ColliderCone(float radius = 1.0f, float height = 1.0f, const Transform &localTransform = {});

	~ColliderCone();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	float GetRadius() const { return m_radius; }

	void SetRadius(float radius);

	float GetHeight() const { return m_height; }

	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, ColliderCone &collider);

	friend Node &operator<<(Node &node, const ColliderCone &collider);

private:
	std::unique_ptr<btConeShape> m_shape;
	float m_radius;
	float m_height;
};
}
