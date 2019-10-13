#include "EmitterSphere.hpp"

#include "Maths/Maths.hpp"
#include "Maths/Vector2.hpp"

namespace acid {
bool EmitterSphere::registered = Register("sphere");

EmitterSphere::EmitterSphere(float radius) :
	m_radius(radius) {
}

Vector3f EmitterSphere::GeneratePosition() const {
	auto a = Maths::Random(0.0f, 1.0f);
	auto b = Maths::Random(0.0f, 1.0f);

	if (a > b) {
		std::swap(a, b);
	}

	auto randX = b * std::cos(2.0f * Maths::Pi<float> * (a / b));
	auto randY = b * std::sin(2.0f * Maths::Pi<float> * (a / b));
	auto distance = Vector2f(randX, randY).Length();
	return m_radius * distance * RandomUnitVector();
}

const Node &operator>>(const Node &node, EmitterSphere &emitter) {
	node["radius"].Get(emitter.m_radius);
	return node;
}

Node &operator<<(Node &node, const EmitterSphere &emitter) {
	node["radius"].Set(emitter.m_radius);
	return node;
}
}
