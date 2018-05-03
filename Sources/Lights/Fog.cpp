#include "Fog.hpp"

namespace fl
{
	Fog::Fog(Colour *colour, const float &density, const float &gradient, const float &lowerLimit, const float &upperLimit) :
		m_colour(colour),
		m_density(density),
		m_gradient(gradient),
		m_lowerLimit(lowerLimit),
		m_upperLimit(upperLimit)
	{
	}

	Fog::~Fog()
	{
		delete m_colour;
	}

	Fog *Fog::Set(const Fog &source)
	{
		*m_colour = *source.m_colour;
		m_density = source.m_density;
		m_gradient = source.m_gradient;
		m_lowerLimit = source.m_lowerLimit;
		m_upperLimit = source.m_upperLimit;
		return this;
	}

	Fog &Fog::operator=(const Fog &other)
	{
		return *Set(other);
	}
}
