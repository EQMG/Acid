#include "Fog.hpp"

namespace Flounder
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

	void Fog::Set(const Fog &source)
	{
		m_colour->Set(*source.m_colour);
		m_density = source.m_density;
		m_gradient = source.m_gradient;
		m_lowerLimit = source.m_lowerLimit;
		m_upperLimit = source.m_upperLimit;
	}
}
