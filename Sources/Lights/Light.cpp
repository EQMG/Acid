#include "Light.hpp"

#include "Scenes/Entity.inl"

namespace acid {
bool Light::registered = Register("light");

Light::Light(const Colour &colour, float radius) :
	m_colour(colour),
	m_radius(radius) {
}

const Node &operator>>(const Node &node, Light &light) {
	node["colour"].Get(light.m_colour);
	node["radius"].Get(light.m_radius);
	return node;
}

Node &operator<<(Node &node, const Light &light) {
	node["colour"].Set(light.m_colour);
	node["radius"].Set(light.m_radius);
	return node;
}
}
