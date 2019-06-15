#include "Light.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
Light::Light(const Colour &colour, const float &radius) :
	m_colour{colour},
	m_radius{radius}
{
}

void Light::Start()
{
}

void Light::Update()
{
}

const Metadata &operator>>(const Metadata &metadata, Light &light)
{
	metadata.GetChild("colour", light.m_colour);
	metadata.GetChild("radius", light.m_radius);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Light &light)
{
	metadata.SetChild("colour", light.m_colour);
	metadata.SetChild("radius", light.m_radius);
	return metadata;
}
}
