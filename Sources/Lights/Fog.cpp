#include "Fog.hpp"

namespace acid {
const bool Fog::Registered = Register("fog");

Fog::Fog(const Colour &colour, float density, float gradient, float lowerLimit, float upperLimit) :
	colour(colour),
	density(density),
	gradient(gradient),
	lowerLimit(lowerLimit),
	upperLimit(upperLimit) {
}

void Fog::Start() {
}

void Fog::Update() {
}

const Node &operator>>(const Node &node, Fog &fog) {
	node["colour"].Get(fog.colour);
	node["density"].Get(fog.density);
	node["gradient"].Get(fog.gradient);
	node["lowerLimit"].Get(fog.lowerLimit);
	node["upperLimit"].Get(fog.upperLimit);
	return node;
}

Node &operator<<(Node &node, const Fog &fog) {
	node["colour"].Set(fog.colour);
	node["density"].Set(fog.density);
	node["gradient"].Set(fog.gradient);
	node["lowerLimit"].Set(fog.lowerLimit);
	node["upperLimit"].Set(fog.upperLimit);
	return node;
}
}
