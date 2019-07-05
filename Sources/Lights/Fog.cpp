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

std::string Fog::ToString() const
{
	std::stringstream stream;
	stream << "Fog(" << m_colour << ", " << m_density << ", " << m_gradient << ", " << m_lowerLimit << ", " << m_upperLimit << ")";
	return stream.str();
}

const Node &operator>>(const Node &node, Fog &fog)
{
	node["colour"].Get(fog.m_colour);
	node["density"].Get(fog.m_density);
	node["gradient"].Get(fog.m_gradient);
	node["lowerLimit"].Get(fog.m_lowerLimit);
	node["upperLimit"].Get(fog.m_upperLimit);
	return node;
}

Node &operator<<(Node &node, const Fog &fog)
{
	node["colour"].Set(fog.m_colour);
	node["density"].Set(fog.m_density);
	node["gradient"].Set(fog.m_gradient);
	node["lowerLimit"].Set(fog.m_lowerLimit);
	node["upperLimit"].Set(fog.m_upperLimit);
	return node;
}

std::ostream &operator<<(std::ostream &stream, const Fog &fog)
{
	stream << fog.ToString();
	return stream;
}
}
