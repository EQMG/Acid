#include "EmitterCircle.hpp"

#include "Scenes/Entity.hpp"

namespace acid {
bool EmitterCircle::registered = Register("circle");

EmitterCircle::EmitterCircle(float radius, const Vector3f &heading) :
	m_radius(radius),
	m_heading(heading.Normalize()) {
}

Vector3f EmitterCircle::GeneratePosition() const {
	Vector3f direction;

	do {
		auto randomVector = RandomUnitVector();
		direction = randomVector.Cross(m_heading);
	} while (direction.Length() == 0.0f);

	direction.Normalize();
	direction *= m_radius;

	auto a = Maths::Random(0.0f, 1.0f);
	auto b = Maths::Random(0.0f, 1.0f);

	if (a > b) {
		std::swap(a, b);
	}

	auto randX = b * std::cos(2.0f * Maths::Pi<float> * (a / b));
	auto randY = b * std::sin(2.0f * Maths::Pi<float> * (a / b));
	auto distance = Vector3f(randX, randY, 0.0f).Length();
	return direction * distance;
}

const Node &operator>>(const Node &node, EmitterCircle &emitter) {
	node["radius"].Get(emitter.m_radius);
	node["heading"].Get(emitter.m_heading);
	return node;
}

Node &operator<<(Node &node, const EmitterCircle &emitter) {
	node["radius"].Set(emitter.m_radius);
	node["heading"].Set(emitter.m_heading);
	return node;
}
}
