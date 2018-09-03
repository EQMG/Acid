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

	Fog::Fog(const Fog &source) :
		m_colour(source.m_colour),
		m_density(source.m_density),
		m_gradient(source.m_gradient),
		m_lowerLimit(source.m_lowerLimit),
		m_upperLimit(source.m_upperLimit)
	{
	}

	Fog::~Fog()
	{
	}

	void Fog::Decode(const Serialized &serialized)
	{
		m_colour = serialized.GetChild<Colour>("Colour");
		m_density = serialized.GetChild<float>("Density");
		m_gradient = serialized.GetChild<float>("Gradient");
		m_lowerLimit = serialized.GetChild<float>("Lower Limit");
		m_upperLimit = serialized.GetChild<float>("Upper Limit");
	}

	void Fog::Encode(Serialized &serialized) const
	{
		serialized.SetChild<Colour>("Colour", m_colour);
		serialized.SetChild<float>("Density", m_density);
		serialized.SetChild<float>("Gradient", m_gradient);
		serialized.SetChild<float>("Lower Limit", m_lowerLimit);
		serialized.SetChild<float>("Upper Limit", m_upperLimit);
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
