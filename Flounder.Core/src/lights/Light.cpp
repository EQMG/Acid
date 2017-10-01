#include "Light.hpp"

namespace Flounder
{
	Light::Light() :
		m_colour(new Colour(1.0f, 1.0f, 1.0f)),
		m_position(new Vector3()),
		m_attenuation(new Attenuation(1.0f, 0.0f, 0.0f))
	{
	}

	Light::Light(const Colour &colour, const Vector3 &position) :
		m_colour(new Colour(colour)),
		m_position(new Vector3(position)),
		m_attenuation(new Attenuation(1.0f, 0.0f, 0.0f))
	{
	}

	Light::Light(const Colour &colour, const Vector3 &position, const Attenuation &attenuation) :
		m_colour(new Colour(colour)),
		m_position(new Vector3(position)),
		m_attenuation(new Attenuation(attenuation))
	{
	}

	Light::Light(const Light &source) :
		m_colour(new Colour(*source.m_colour)),
		m_position(new Vector3(*source.m_position)),
		m_attenuation(new Attenuation(*source.m_attenuation))
	{
	}

	Light::~Light()
	{
		delete m_colour;
		delete m_position;
		delete m_attenuation;
	}

	Light *Light::Set(const Colour &colour, const Vector3 &position)
	{
		m_colour->Set(colour);
		m_position->Set(position);
		return this;
	}

	Light *Light::Set(const Colour &colour, const Vector3 &position, const Attenuation &attenuation)
	{
		m_colour->Set(colour);
		m_position->Set(position);
		m_attenuation->Set(attenuation);
		return this;
	}

	Light *Light::Set(const Light &source)
	{
		m_colour->Set(*source.m_colour);
		m_position->Set(*source.m_position);
		m_attenuation->Set(*source.m_attenuation);
		return this;
	}
}
