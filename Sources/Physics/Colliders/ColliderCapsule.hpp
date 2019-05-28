#pragma once

#include "Collider.hpp"

class btCapsuleShape;

namespace acid
{
class ColliderCapsule :
	public Collider
{
public:
	explicit ColliderCapsule(const float &radius = 0.5f, const float &height = 1.0f, const Transform &localTransform = Transform::Zero);

	~ColliderCapsule();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius);

	const float &GetHeight() const { return m_height; }

	void SetHeight(const float &height);

	friend const Metadata &operator>>(const Metadata &metadata, ColliderCapsule &collider);

	friend Metadata &operator<<(Metadata &metadata, const ColliderCapsule &collider);

private:
	std::unique_ptr<btCapsuleShape> m_shape;
	float m_radius;
	float m_height;
};
}
