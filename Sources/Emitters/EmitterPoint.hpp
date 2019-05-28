#pragma once

#include "Emitter.hpp"

namespace acid
{
class EmitterPoint :
	public Emitter
{
public:
	EmitterPoint();

	void Start() override;

	void Update() override;

	Vector3f GeneratePosition() const override;

	const Vector3f &GetPoint() const { return m_point; }

	void SetPoint(const Vector3f &point) { m_point = point; }

	friend const Metadata &operator>>(const Metadata &metadata, EmitterPoint &emitter);

	friend Metadata &operator<<(Metadata &metadata, const EmitterPoint &emitter);

private:
	Vector3f m_point;
};
}
