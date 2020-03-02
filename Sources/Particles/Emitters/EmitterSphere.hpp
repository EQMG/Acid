#pragma once

#include "Emitter.hpp"

namespace acid {
class ACID_EXPORT EmitterSphere : public Emitter::Registrar<EmitterSphere> {
	inline static const bool Registered = Register("sphere");
public:
	explicit EmitterSphere(float radius = 1.0f);

	Vector3f GeneratePosition() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius) { this->radius = radius; }

	friend const Node &operator>>(const Node &node, EmitterSphere &emitter);
	friend Node &operator<<(Node &node, const EmitterSphere &emitter);

private:
	float radius;
};
}
