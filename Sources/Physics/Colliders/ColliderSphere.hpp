#pragma once

#include "Collider.hpp"

class btSphereShape;

namespace acid
{
class ACID_EXPORT ColliderSphere :
	public Collider
{
public:
	explicit ColliderSphere(const float &radius = 0.5f, const Transform &localTransform = Transform());

	~ColliderSphere();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius);

	friend const Metadata &operator>>(const Metadata &metadata, ColliderSphere &collider);

	friend Metadata &operator<<(Metadata &metadata, const ColliderSphere &collider);

private:
	std::unique_ptr<btSphereShape> m_shape;
	float m_radius;
};
}
