#pragma once

#include "Emitter.hpp"

namespace acid {
class ACID_SHIM_EXPORT SphereEmitter : public Emitter::Registrar<SphereEmitter> {
	inline static const bool Registered = Register("sphere");
public:
	explicit SphereEmitter(float radius = 1.0f);

	Vector3f GeneratePosition() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius) { this->radius = radius; }

	friend const Node &operator>>(const Node &node, SphereEmitter &emitter);
	friend Node &operator<<(Node &node, const SphereEmitter &emitter);

private:
	float radius;
};
}
