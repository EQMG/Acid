#pragma once

#include "Emitter.hpp"

namespace acid {
class ACID_EXPORT EmitterLine : public Emitter::Registrar<EmitterLine> {
	inline static const bool Registered = Register("line");
public:
	explicit EmitterLine(float length = 1.0f, const Vector3f &axis = Vector3f::Right);

	Vector3f GeneratePosition() const override;

	float GetLength() const { return length; }
	void SetLength(float length) { this->length = length; }

	const Vector3f &GetAxis() const { return axis; }
	void SetAxis(const Vector3f &axis) { this->axis = axis; }

	friend const Node &operator>>(const Node &node, EmitterLine &emitter);
	friend Node &operator<<(Node &node, const EmitterLine &emitter);

private:
	float length;
	Vector3f axis;
};
}
