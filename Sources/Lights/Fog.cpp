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

	void Fog::Decode(const Node &node)
	{
		m_colour = node.GetChild<Colour>("Colour");
		m_density = node.GetChild<float>("Density");
		m_gradient = node.GetChild<float>("Gradient");
		m_lowerLimit = node.GetChild<float>("Lower Limit");
		m_upperLimit = node.GetChild<float>("Upper Limit");
	}

	void Fog::Encode(Node &node) const
	{
		node.SetChild<Colour>("Colour", m_colour);
		node.SetChild<float>("Density", m_density);
		node.SetChild<float>("Gradient", m_gradient);
		node.SetChild<float>("Lower Limit", m_lowerLimit);
		node.SetChild<float>("Upper Limit", m_upperLimit);
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
