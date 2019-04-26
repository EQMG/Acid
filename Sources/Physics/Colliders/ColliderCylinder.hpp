#pragma once

#include "Collider.hpp"

class btCylinderShape;

namespace acid
{
class ACID_EXPORT ColliderCylinder :
	public Collider
{
public:
	explicit ColliderCylinder(const float &radius = 1.0f, const float &height = 1.0f, const Transform &localTransform = Transform::Zero);

	~ColliderCylinder();

	void Start() override;

	void Update() override;

	btCollisionShape *GetCollisionShape() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius);

	const float &GetHeight() const { return m_height; }

	void SetHeight(const float &height);

	ACID_EXPORT friend const Metadata& operator>>(const Metadata& metadata, ColliderCylinder& collider);

	ACID_EXPORT friend Metadata& operator<<(Metadata& metadata, const ColliderCylinder& collider);

private:
	std::unique_ptr<btCylinderShape> m_shape;
	float m_radius;
	float m_height;
};
}
