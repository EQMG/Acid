#include "ParticleType.hpp"

#include "Resources/Resources.hpp"

namespace acid
{
	std::shared_ptr<ParticleType> ParticleType::Resource(const std::shared_ptr<Texture> &texture, const uint32_t &numberOfRows, const Colour &colourOffset, const float &lifeLength, const float &scale)
	{
		auto resource = Resources::Get()->Get(ToFilename(texture, numberOfRows, colourOffset, lifeLength, scale));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<ParticleType>(resource);
		}

		auto result = std::make_shared<ParticleType>(texture, numberOfRows, colourOffset, lifeLength, scale);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<ParticleType> ParticleType::Resource(const std::string &data)
	{
		auto split = FormatString::Split(data, "_");
		auto texture = Texture::Resource(split[1]);
		uint32_t numberOfRows = static_cast<uint32_t>(atof(split[2].c_str()));
		Colour colourOffset = Colour(split[3]);
		float lifeLength = static_cast<float>(atof(split[4].c_str()));
		float scale = static_cast<float>(atof(split[5].c_str()));
		return Resource(texture, numberOfRows, colourOffset, lifeLength, scale);
	}

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
