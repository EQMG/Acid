#include "light.h"

namespace flounder {
	light::light(colour *colour, vector3 *position)
	{
		m_colour = colour;
		m_position = position;
		m_attenuation = new attenuation(1.0f, 0.0f, 0.0f);
	}

	light::light(colour *colour, vector3 *position, attenuation *attenuation)
	{
		m_colour = colour;
		m_position = position;
		m_attenuation = attenuation;
	}

	light::~light()
	{
		delete m_colour;
		delete m_position;
		delete m_attenuation;
	}
}
