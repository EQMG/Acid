#include "FontType.hpp"

#include "Resources/Resources.hpp"

namespace acid
{
	std::shared_ptr<FontType> FontType::Create(const Metadata &metadata)
	{
		std::shared_ptr<Resource> resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<FontType>(resource);
		}

		auto filename = metadata.GetChild<std::string>("Filename");
		auto style = metadata.GetChild<std::string>("Style");
		auto result = std::make_shared<FontType>(filename, style);
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	std::shared_ptr<FontType> FontType::Create(const std::string &filename, const std::string &style)
	{
		Metadata metadata = Metadata();
		metadata.SetChild<std::string>("Filename", filename);
		metadata.SetChild<std::string>("Style", style);
		return Create(metadata);
	}

	FontType::FontType(const std::string &filename, const std::string &style) :
		m_filename(filename),
		m_style(style),
		m_texture(Texture::Create(m_filename + "/" + m_style + ".png")),
		m_metadata(FontMetafile(m_filename + "/" + m_style + ".fnt"))
	{
	}

	void FontType::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Filename", m_filename);
		metadata.SetChild<std::string>("Style", m_style);
	}
}
