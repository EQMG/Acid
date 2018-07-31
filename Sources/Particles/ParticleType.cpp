#include "ParticleType.hpp"

namespace acid
{
	ParticleType::ParticleType(std::shared_ptr<Texture> texture, const Colour &colourOffset, const float &lifeLength, const float &scale) :
		m_filename(ToFilename(texture, colourOffset, lifeLength, scale)),
		m_texture(texture),
		m_colourOffset(colourOffset),
		m_lifeLength(lifeLength),
		m_scale(scale)
	{
	}

	ParticleType::~ParticleType()
	{
	}

	std::string ParticleType::ToFilename(const std::shared_ptr<Texture> &texture, const Colour &colourOffset, const float &lifeLength, const float &scale)
	{
		return "ParticleType_" + texture->GetName() + "_" + colourOffset.GetHex() + "_" + std::to_string(lifeLength) + "_" + std::to_string(scale);
	}
}
