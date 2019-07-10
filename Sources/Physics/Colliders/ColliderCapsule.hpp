#pragma once

#include "Collider.hpp"

class btCapsuleShape;

namespace acid
{
class ACID_EXPORT ColliderCapsule :
	public Collider
{
public:
	explicit ColliderCapsule(const float &radius = 0.5f, const float &height = 1.0f, const Transform &localTransform = {});

	~ColliderCapsule();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius);

	const float &GetHeight() const { return m_height; }

	void SetHeight(const float &height);

	friend const Node &operator>>(const Node &node, ColliderCapsule &collider);

	friend Node &operator<<(Node &node, const ColliderCapsule &collider);

private:
	std::unique_ptr<btCapsuleShape> m_shape;
	float m_radius;
	float m_height;
};
}
