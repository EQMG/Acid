#include "Light.hpp"

#include "Scenes/Entity.hpp"

namespace acid {
const bool Light::Registered = Register("light");

Light::Light(const Colour &colour, float radius) :
	colour(colour),
	radius(radius) {
}

void Light::Start() {
}

void Light::Update() {
}

const Node &operator>>(const Node &node, Light &light) {
	node["colour"].Get(light.colour);
	node["radius"].Get(light.radius);
	return node;
}

Node &operator<<(Node &node, const Light &light) {
	node["colour"].Set(light.colour);
	node["radius"].Set(light.radius);
	return node;
}
}
