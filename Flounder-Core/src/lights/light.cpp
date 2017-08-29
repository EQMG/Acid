#include "light.hpp"

namespace flounder
{
	light::light(colour *colour, vector3 *position)
	{
		set(colour, position, new attenuation(1.0f, 0.0f, 0.0f));
	}

	light::light(colour *colour, vector3 *position, attenuation *attenuation)
	{
		set(colour, position, attenuation);
	}

	light::~light()
	{
		delete m_colour;
		delete m_position;
		delete m_attenuation;
	}

	light *light::set(colour *colour, vector3 *position)
	{
		m_colour = colour;
		m_position = position;
		return this;
	}

	light *light::set(colour *colour, vector3 *position, attenuation *attenuation)
	{
		m_colour = colour;
		m_position = position;
		m_attenuation = attenuation;
		return this;
	}
}
