#pragma once

#include "Collider.hpp"

class btCapsuleShape;

namespace acid
{
class ACID_EXPORT ColliderCapsule :
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

	ACID_EXPORT friend const Metadata& operator>>(const Metadata& metadata, ColliderCapsule& collider);

	ACID_EXPORT friend Metadata& operator<<(Metadata& metadata, const ColliderCapsule& collider);

private:
	btCapsuleShape *m_shape;
	float m_radius;
	float m_height;
};
}
