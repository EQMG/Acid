#include "Fog.hpp"

namespace acid
{
Fog::Fog(const Colour& colour, const float& density, const float& gradient, const float& lowerLimit, const float& upperLimit) : m_colour(colour), m_density(density), m_gradient(gradient), m_lowerLimit(lowerLimit), m_upperLimit(upperLimit) {}

void Fog::Decode(const Metadata& metadata)
{
	metadata.GetChild("Colour", m_colour);
	metadata.GetChild("Density", m_density);
	metadata.GetChild("Gradient", m_gradient);
	metadata.GetChild("Lower Limit", m_lowerLimit);
	metadata.GetChild("Upper Limit", m_upperLimit);
}

void Fog::Encode(Metadata& metadata) const
{
	metadata.SetChild("Colour", m_colour);
	metadata.SetChild("Density", m_density);
	metadata.SetChild("Gradient", m_gradient);
	metadata.SetChild("Lower Limit", m_lowerLimit);
	metadata.SetChild("Upper Limit", m_upperLimit);
}

std::ostream& operator<<(std::ostream& stream, const Fog& colour)
{
	stream << colour.ToString();
	return stream;
}

std::string Fog::ToString() const
{
	std::stringstream result;
	result << "Fog(" << m_colour << ", " << m_density << ", " << m_gradient << ", " << m_lowerLimit << ", " << m_upperLimit << ")";
	return result.str();
}
}
