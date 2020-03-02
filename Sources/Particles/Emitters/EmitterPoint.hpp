#pragma once

#include "Emitter.hpp"

namespace acid {
class ACID_EXPORT EmitterPoint : public Emitter::Registrar<EmitterPoint> {
	inline static const bool Registered = Register("point");
public:
	EmitterPoint();

	Vector3f GeneratePosition() const override;

	const Vector3f &GetPoint() const { return point; }
	void SetPoint(const Vector3f &point) { this->point = point; }

	friend const Node &operator>>(const Node &node, EmitterPoint &emitter);
	friend Node &operator<<(Node &node, const EmitterPoint &emitter);

private:
	Vector3f point;
};
}
