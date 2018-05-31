#include "ParticleType.hpp"

namespace fl
{
	ParticleType::ParticleType(const std::string &name, std::shared_ptr<Texture> texture, const float &lifeLength, const float &scale) :
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
