#pragma once

#include "Emitter.hpp"

namespace acid {
class ACID_EXPORT EmitterPoint : public Emitter {
public:
	EmitterPoint();

	void Start() override;
	void Update() override;
	Vector3f GeneratePosition() const override;

	const Vector3f &GetPoint() const { return m_point; }
	void SetPoint(const Vector3f &point) { m_point = point; }

	friend const Node &operator>>(const Node &node, EmitterPoint &emitter);
	friend Node &operator<<(Node &node, const EmitterPoint &emitter);

private:
	static Registrar<EmitterPoint> registered;

	Vector3f m_point;
};
}
