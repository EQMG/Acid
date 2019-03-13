#pragma once

#include "Collider.hpp"

class btCapsuleShape;

namespace acid
{
class ACID_EXPORT ColliderCapsule : public Collider
{
  public:
	explicit ColliderCapsule(const float& radius = 0.5f, const float& height = 1.0f, const Transform& localTransform = Transform::Identity);

	~ColliderCapsule();

	void Start() override;

	void Update() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

	btCollisionShape* GetCollisionShape() const override;

	const float& GetRadius() const
	{
		return m_radius;
	}

	void SetRadius(const float& radius);

	const float& GetHeight() const
	{
		return m_height;
	}

	void SetHeight(const float& height);

  private:
	btCapsuleShape* m_shape;
	float m_radius;
	float m_height;
};
}
