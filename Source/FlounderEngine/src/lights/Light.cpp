#include "Light.hpp"

namespace Flounder
{
	Light::Light() :
		m_colour(new Colour(1.0f, 1.0f, 1.0f)),
		m_radius(-1.0f),
		m_position(new Vector3())
	{
	}

	Light::Light(const Colour &colour, const float &radius, const Vector3 &position) :
		m_colour(new Colour(colour)),
		m_radius(radius),
		m_position(new Vector3(position))
	{
	}

	Light::Light(const Light &source) :
		m_colour(new Colour(*source.m_colour)),
		m_radius(source.m_radius),
		m_position(new Vector3(*source.m_position))
	{
	}

	Light::~Light()
	{
		delete m_colour;
		delete m_position;
	}

	Light *Light::Set(const Colour &colour, const Vector3 &position)
	{
		m_colour->Set(colour);
		m_position->Set(position);
		return this;
	}

	Light *Light::Set(const Colour &colour, const float &radius, const Vector3 &position)
	{
		m_colour->Set(colour);
		m_radius = radius;
		m_position->Set(position);
		return this;
	}

	Light *Light::Set(const Light &source)
	{
		m_colour->Set(*source.m_colour);
		m_radius = source.m_radius;
		m_position->Set(*source.m_position);
		return this;
	}
}
