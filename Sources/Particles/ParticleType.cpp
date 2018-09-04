#include "ParticleType.hpp"

namespace acid
{
	ParticleType::ParticleType(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale) :
		m_filename(ToFilename(texture, numberOfRows, colourOffset, lifeLength, scale)),
		m_texture(texture),
		m_numberOfRows(numberOfRows),
		m_colourOffset(colourOffset),
		m_lifeLength(lifeLength),
		m_scale(scale)
	{
	}

	ParticleType::ParticleType(const ParticleType &source) :
		m_filename(source.m_filename),
		m_texture(source.m_texture),
		m_numberOfRows(source.m_numberOfRows),
		m_colourOffset(source.m_colourOffset),
		m_lifeLength(source.m_lifeLength),
		m_scale(source.m_scale)
	{
	}

	ParticleType::~ParticleType()
	{
	}

	void ParticleType::Decode(const Node &node)
	{
		TrySetTexture(node.GetChild<std::string>("Texture"));
		m_numberOfRows = node.GetChild<uint32_t>("Number Of Rows");
		m_colourOffset = node.GetChild<Colour>("Colour Offset");
		m_lifeLength = node.GetChild<float>("Life Length");
		m_scale = node.GetChild<float>("Scale");
		m_filename = ToFilename(m_texture, m_numberOfRows, m_colourOffset, m_lifeLength, m_scale);
	}

	void ParticleType::Encode(Node &node) const
	{
		node.SetChild<std::string>("Texture", m_texture == nullptr ? "" : m_texture->GetFilename());
		node.SetChild<uint32_t>("Number Of Rows", m_numberOfRows);
		node.SetChild<Colour>("Colour Offset", m_colourOffset);
		node.SetChild<float>("Life Length", m_lifeLength);
		node.SetChild<float>("Scale", m_scale);
	}

	std::string ParticleType::ToFilename(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale)
	{
		std::stringstream result;
		result << "ParticleType_" << (texture == nullptr ? "nullptr" : texture->GetFilename()) << "_" << numberOfRows << "_" << colourOffset.GetHex() << "_" << lifeLength << "_" << scale;
		return result.str();
	}
}
