#pragma once

#include "Emitter.hpp"

namespace acid {
class ACID_EXPORT EmitterCircle : public Emitter::Registrar<EmitterCircle> {
	inline static const bool Registered = Register("circle");
public:
	explicit EmitterCircle(float radius = 1.0f, const Vector3f &heading = Vector3f::Up);

	Vector3f GeneratePosition() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius) { this->radius = radius; }

	const Vector3f &GetHeading() const { return heading; }
	void SetHeading(const Vector3f &heading) { this->heading = heading; }

	friend const Node &operator>>(const Node &node, EmitterCircle &emitter);
	friend Node &operator<<(Node &node, const EmitterCircle &emitter);

private:
	float radius;
	Vector3f heading;
};
}
