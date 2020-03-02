#pragma once

#include "Emitter.hpp"

namespace acid {
class ACID_EXPORT CircleEmitter : public Emitter::Registrar<CircleEmitter> {
	inline static const bool Registered = Register("circle");
public:
	explicit CircleEmitter(float radius = 1.0f, const Vector3f &heading = Vector3f::Up);

	Vector3f GeneratePosition() const override;

	float GetRadius() const { return radius; }
	void SetRadius(float radius) { this->radius = radius; }

	const Vector3f &GetHeading() const { return heading; }
	void SetHeading(const Vector3f &heading) { this->heading = heading; }

	friend const Node &operator>>(const Node &node, CircleEmitter &emitter);
	friend Node &operator<<(Node &node, const CircleEmitter &emitter);

private:
	float radius;
	Vector3f heading;
};
}
