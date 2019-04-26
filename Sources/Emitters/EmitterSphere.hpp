#pragma once

#include "Emitter.hpp"

namespace acid
{
class ACID_EXPORT EmitterSphere :
	public Emitter
{
public:
	explicit EmitterSphere(const float &radius = 1.0f, const Transform &localTransform = Transform::Zero);

	void Start() override;

	void Update() override;

	Vector3f GeneratePosition() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius) { m_radius = radius; }

	ACID_EXPORT friend const Metadata& operator>>(const Metadata& metadata, EmitterSphere& emitter);

	ACID_EXPORT friend Metadata& operator<<(Metadata& metadata, const EmitterSphere& emitter);

private:
	float m_radius;
};
}
