#include "particletype.hpp"

namespace flounder
{
	particletype::particletype(const std::string &name, texture *texture, const float &lifeLength, const float &scale)
	{
		m_name = name;
		m_texture = texture;
		m_lifeLength = lifeLength;
		m_scale = scale;
	}

	particletype::~particletype()
	{
		// Keeps the texture as the texture manager will delete it if it's inactive.
	}
}
