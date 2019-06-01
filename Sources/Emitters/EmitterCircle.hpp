#pragma once

#include "Emitter.hpp"

namespace acid
{
class ACID_EXPORT EmitterCircle :
	public Emitter
{
public:
	explicit EmitterCircle(const float &radius = 1.0f, const Vector3f &heading = Vector3f::Up);

	void Start() override;

	void Update() override;

	Vector3f GeneratePosition() const override;

	const float &GetRadius() const { return m_radius; }

	void SetRadius(const float &radius) { m_radius = radius; }

	const Vector3f &GetHeading() const { return m_heading; }

	void SetHeading(const Vector3f &heading) { m_heading = heading; }

	friend const Metadata &operator>>(const Metadata &metadata, EmitterCircle &emitter);

	friend Metadata &operator<<(Metadata &metadata, const EmitterCircle &emitter);

private:
	float m_radius;
	Vector3f m_heading;
};
}
