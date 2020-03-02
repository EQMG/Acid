#include "SphereEmitter.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Vector2.hpp"

namespace acid {
SphereEmitter::SphereEmitter(float radius) :
	radius(radius) {
}

Vector3f SphereEmitter::GeneratePosition() const {
	auto a = Maths::Random(0.0f, 1.0f);
	auto b = Maths::Random(0.0f, 1.0f);

	if (a > b) {
		std::swap(a, b);
	}

	auto randX = b * std::cos(2.0f * Maths::PI<float> * (a / b));
	auto randY = b * std::sin(2.0f * Maths::PI<float> * (a / b));
	auto distance = Vector2f(randX, randY).Length();
	return radius * distance * RandomUnitVector();
}

const Node &operator>>(const Node &node, SphereEmitter &emitter) {
	node["radius"].Get(emitter.radius);
	return node;
}

Node &operator<<(Node &node, const SphereEmitter &emitter) {
	node["radius"].Set(emitter.radius);
	return node;
}
}
