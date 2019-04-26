#include "Fog.hpp"

namespace acid
{
Fog::Fog(const Colour &colour, const float &density, const float &gradient, const float &lowerLimit, const float &upperLimit) :
	m_colour(colour),
	m_density(density),
	m_gradient(gradient),
	m_lowerLimit(lowerLimit),
	m_upperLimit(upperLimit)
{
}

std::string Fog::ToString() const
{
	std::stringstream stream;
	stream << "Fog(" << m_colour << ", " << m_density << ", " << m_gradient << ", " << m_lowerLimit << ", " << m_upperLimit << ")";
	return stream.str();
}

const Metadata& operator>>(const Metadata& metadata, Fog& fog)
{
	metadata.GetChild("Colour", fog.m_colour);
	metadata.GetChild("Density", fog.m_density);
	metadata.GetChild("Gradient", fog.m_gradient);
	metadata.GetChild("Lower Limit", fog.m_lowerLimit);
	metadata.GetChild("Upper Limit", fog.m_upperLimit);
	return metadata;
}

Metadata& operator<<(Metadata& metadata, const Fog& fog)
{
	metadata.SetChild("Colour", fog.m_colour);
	metadata.SetChild("Density", fog.m_density);
	metadata.SetChild("Gradient", fog.m_gradient);
	metadata.SetChild("Lower Limit", fog.m_lowerLimit);
	metadata.SetChild("Upper Limit", fog.m_upperLimit);
	return metadata;
}

std::ostream &operator<<(std::ostream &stream, const Fog &colour)
{
	stream << colour.ToString();
	return stream;
}
}
