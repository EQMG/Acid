#pragma once

#include "Emitter.hpp"

namespace acid
{
class ACID_EXPORT EmitterSphere :
	public Emitter
{
public:
	explicit EmitterSphere(const float &radius = 1.0f);

	void Start() override;

	void Update() override;

	Vector3f GeneratePosition() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius) { m_radius = radius; }

	friend const Node &operator>>(const Node &node, EmitterSphere &emitter);

	friend Node &operator<<(Node &node, const EmitterSphere &emitter);

private:
	float m_radius;
};
}
