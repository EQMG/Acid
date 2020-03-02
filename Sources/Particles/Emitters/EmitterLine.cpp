#include "EmitterLine.hpp"

#include "Maths/Maths.hpp"
#include "Scenes/Entity.hpp"

namespace acid {
EmitterLine::EmitterLine(float length, const Vector3f &axis) :
	length(length),
	axis(axis.Normalize()) {
}

Vector3f EmitterLine::GeneratePosition() const {
	return axis * length * Maths::Random(-0.5f, 0.5f);
}

const Node &operator>>(const Node &node, EmitterLine &emitter) {
	node["length"].Get(emitter.length);
	node["axis"].Get(emitter.axis);
	return node;
}

Node &operator<<(Node &node, const EmitterLine &emitter) {
	node["length"].Set(emitter.length);
	node["axis"].Set(emitter.axis);
	return node;
}
}
