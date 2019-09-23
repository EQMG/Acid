#pragma once

#include "Collider.hpp"

class btCapsuleShape;

namespace acid {
class ACID_EXPORT ColliderCapsule : public Component::Registrar<ColliderCapsule>, public Collider {
public:
	explicit ColliderCapsule(float radius = 0.5f, float height = 1.0f, const Transform &localTransform = {});

	~ColliderCapsule();

	void Start() override;
	void Update() override;

	btCollisionShape *GetCollisionShape() const override;
	void SetLocalTransform(const Transform &localTransform) override;

	float GetRadius() const { return m_radius; }
	void SetRadius(float radius);

	float GetHeight() const { return m_height; }
	void SetHeight(float height);

	friend const Node &operator>>(const Node &node, ColliderCapsule &collider);
	friend Node &operator<<(Node &node, const ColliderCapsule &collider);

private:
	static bool registered;

	std::unique_ptr<btCapsuleShape> m_shape;
	float m_radius;
	float m_height;
};
}
