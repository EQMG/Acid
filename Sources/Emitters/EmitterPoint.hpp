#pragma once

#include "Emitter.hpp"

namespace acid
{
class ACID_EXPORT EmitterPoint :
	public Emitter
{
public:
	EmitterPoint();

	void Start() override;

	void Update() override;

	Vector3f GeneratePosition() const override;

	const Vector3f &GetPoint() const { return m_point; }

	void SetPoint(const Vector3f &point) { m_point = point; }

	ACID_EXPORT friend const Metadata &operator>>(const Metadata &metadata, EmitterPoint &emitter);

	ACID_EXPORT friend Metadata &operator<<(Metadata &metadata, const EmitterPoint &emitter);

private:
	Vector3f m_point;
};
}
