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

	Fog::~Fog()
	{
	}

	void Fog::Write(LoadedValue &destination)
	{
		destination.FindChild("colour", true)->SetString(m_colour.GetHex());
		destination.SetChild<float>("density", m_density);
		destination.SetChild<float>("gradient", m_gradient);
		destination.SetChild<float>("lowerLimit", m_lowerLimit);
		destination.SetChild<float>("upperLimit", m_upperLimit);
	}

	Fog &Fog::operator=(const Fog &other)
	{
		m_colour = other.m_colour;
		m_density = other.m_density;
		m_gradient = other.m_gradient;
		m_lowerLimit = other.m_lowerLimit;
		m_upperLimit = other.m_upperLimit;
		return *this;
	}

	Fog &Fog::operator=(LoadedValue &value)
	{
		m_colour = value.FindChild("colour")->GetString();
		m_density = value.FindChild("density")->Get<float>();
		m_gradient = value.FindChild("gradient")->Get<float>();
		m_lowerLimit = value.FindChild("lowerLimit")->Get<float>();
		m_upperLimit = value.FindChild("upperLimit")->Get<float>();
		return *this;
	}
}
