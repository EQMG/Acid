#include "light.hpp"

namespace Flounder
{
	Light::Light(Colour *colour, Vector3 *position)
	{
		Set(colour, position, new Attenuation(1.0f, 0.0f, 0.0f));
	}

	Light::Light(Colour *colour, Vector3 *position, Attenuation *attenuation)
	{
		Set(colour, position, attenuation);
	}

	Light::~Light()
	{
		delete m_colour;
		delete m_position;
		delete m_attenuation;
	}

	Light *Light::Set(Colour *colour, Vector3 *position)
	{
		m_colour = colour;
		m_position = position;
		return this;
	}

	Light *Light::Set(Colour *colour, Vector3 *position, Attenuation *attenuation)
	{
		m_colour = colour;
		m_position = position;
		m_attenuation = attenuation;
		return this;
	}
}
