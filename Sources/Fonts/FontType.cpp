#include "FontType.hpp"

#include "Resources/Resources.hpp"

namespace acid
{
	std::shared_ptr<FontType> FontType::Create(const std::string &filename, const std::string &fontStyle)
	{
		if (filename.empty())
		{
			return nullptr;
		}

		auto resource = Resources::Get()->Find(ToFilename(filename, fontStyle));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<FontType>(resource);
		}

		auto result = std::make_shared<FontType>(filename, fontStyle);
		Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	std::shared_ptr<FontType> FontType::Create(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		auto split = String::Split(data, "_");
		std::string filename = split[1];
		std::string fontStyle = split[2];
		return Create(filename, fontStyle);
	}

	FontType::FontType(const std::string &filename, const std::string &style) :
		Resource(ToFilename(filename, style)),
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

	std::string FontType::ToFilename(const std::string &filename, const std::string &fontStyle)
	{
		return "FontType_" + filename + "_" + fontStyle;
	}
}
