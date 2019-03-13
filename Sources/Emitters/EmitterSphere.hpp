#pragma once

#include "Emitter.hpp"

namespace acid
{
class ACID_EXPORT EmitterSphere : public Emitter
{
  public:
	explicit EmitterSphere(const float& radius = 1.0f, const Transform& localTransform = Transform::Identity);

	void Start() override;

	void Update() override;

	void Decode(const Metadata& metadata) override;

	void Encode(Metadata& metadata) const override;

	Vector3 GeneratePosition() const override;

	const float& GetRadius() const
	{
		return m_radius;
	}

	void SetRadius(const float& radius)
	{
		m_radius = radius;
	}

  private:
	float m_radius;
};
}
