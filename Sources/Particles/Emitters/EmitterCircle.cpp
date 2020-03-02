#include "EmitterCircle.hpp"

#include "Scenes/Entity.hpp"

namespace acid {
EmitterCircle::EmitterCircle(float radius, const Vector3f &heading) :
	radius(radius),
	heading(heading.Normalize()) {
}

Vector3f EmitterCircle::GeneratePosition() const {
	Vector3f direction;

	do {
		auto randomVector = RandomUnitVector();
		direction = randomVector.Cross(heading);
	} while (direction.Length() == 0.0f);

	direction.Normalize();
	direction *= radius;

	auto a = Maths::Random(0.0f, 1.0f);
	auto b = Maths::Random(0.0f, 1.0f);

	if (a > b) {
		std::swap(a, b);
	}

	auto randX = b * std::cos(2.0f * Maths::PI<float> * (a / b));
	auto randY = b * std::sin(2.0f * Maths::PI<float> * (a / b));
	auto distance = Vector3f(randX, randY, 0.0f).Length();
	return direction * distance;
}

const Node &operator>>(const Node &node, EmitterCircle &emitter) {
	node["radius"].Get(emitter.radius);
	node["heading"].Get(emitter.heading);
	return node;
}

Node &operator<<(Node &node, const EmitterCircle &emitter) {
	node["radius"].Set(emitter.radius);
	node["heading"].Set(emitter.heading);
	return node;
}
}
