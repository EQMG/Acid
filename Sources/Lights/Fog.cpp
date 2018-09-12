#include "Fog.hpp"

#include "Serialized/Metadata.hpp"

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

	Fog::Fog(const Fog &source) :
		m_colour(source.m_colour),
		m_density(source.m_density),
		m_gradient(source.m_gradient),
		m_lowerLimit(source.m_lowerLimit),
		m_upperLimit(source.m_upperLimit)
	{
	}

	void Fog::Decode(const Metadata &metadata)
	{
		m_colour = metadata.GetChild<Colour>("Colour");
		m_density = metadata.GetChild<float>("Density");
		m_gradient = metadata.GetChild<float>("Gradient");
		m_lowerLimit = metadata.GetChild<float>("Lower Limit");
		m_upperLimit = metadata.GetChild<float>("Upper Limit");
	}

	void Fog::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Colour>("Colour", m_colour);
		metadata.SetChild<float>("Density", m_density);
		metadata.SetChild<float>("Gradient", m_gradient);
		metadata.SetChild<float>("Lower Limit", m_lowerLimit);
		metadata.SetChild<float>("Upper Limit", m_upperLimit);
	}

	std::ostream &operator<<(std::ostream &stream, const Fog &colour)
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
