#pragma once

#include "Collider.hpp"

class btConeShape;

namespace acid
{
class ColliderCone :
	public Collider
{
public:
	explicit ColliderCone(const float &radius = 1.0f, const float &height = 1.0f, const Transform &localTransform = Transform::Zero);

	~ColliderCone();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius);

	const float &GetHeight() const { return m_height; }

	void SetHeight(const float &height);

	friend const Metadata &operator>>(const Metadata &metadata, ColliderCone &collider);

	friend Metadata &operator<<(Metadata &metadata, const ColliderCone &collider);

private:
	std::unique_ptr<btConeShape> m_shape;
	float m_radius;
	float m_height;
};
}
