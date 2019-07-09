#include "Fog.hpp"

namespace acid
{
Fog::Fog(const Colour &colour, const float &density, const float &gradient, const float &lowerLimit, const float &upperLimit) :
	m_colour{colour},
	m_density{density},
	m_gradient{gradient},
	m_lowerLimit{lowerLimit},
	m_upperLimit{upperLimit}
{
}

void Fog::Start()
{
}

void Fog::Update()
{
}

const Metadata &operator>>(const Metadata &metadata, Fog &fog)
{
	metadata.GetChild("colour", fog.m_colour);
	metadata.GetChild("density", fog.m_density);
	metadata.GetChild("gradient", fog.m_gradient);
	metadata.GetChild("lowerLimit", fog.m_lowerLimit);
	metadata.GetChild("upperLimit", fog.m_upperLimit);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Fog &fog)
{
	metadata.SetChild("colour", fog.m_colour);
	metadata.SetChild("density", fog.m_density);
	metadata.SetChild("gradient", fog.m_gradient);
	metadata.SetChild("lowerLimit", fog.m_lowerLimit);
	metadata.SetChild("upperLimit", fog.m_upperLimit);
	return metadata;
}
}
