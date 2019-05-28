#pragma once

#include "Emitter.hpp"

namespace acid
{
class EmitterSphere :
	public Emitter
{
public:
	explicit EmitterSphere(const float &radius = 1.0f);

	void Start() override;

	void Update() override;

	Vector3f GeneratePosition() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius) { m_radius = radius; }

	friend const Metadata &operator>>(const Metadata &metadata, EmitterSphere &emitter);

	friend Metadata &operator<<(Metadata &metadata, const EmitterSphere &emitter);

private:
	float m_radius;
};
}
