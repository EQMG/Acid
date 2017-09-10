#include "ParticleType.hpp"

namespace Flounder
{
	ParticleType::ParticleType(const std::string &name, Texture *texture, const float &lifeLength, const float &scale) :
		m_name(name),
		m_texture(texture),
		m_lifeLength(lifeLength),
		m_scale(scale)
	{
	}

	ParticleType::~ParticleType()
	{
	}
}
