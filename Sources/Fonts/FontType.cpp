#include "FontType.hpp"

#include "Resources/Resources.hpp"

namespace acid
{
	std::shared_ptr<FontType> FontType::Resource(const std::string &filename, const std::string &fontStyle)
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
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	std::shared_ptr<FontType> FontType::Resource(const std::string &data)
	{
		if (data.empty())
		{
			return nullptr;
		}

		auto split = String::Split(data, "_");
		std::string filename = split[1];
		std::string fontStyle = split[2];
		return Resource(filename, fontStyle);
	}

	FontType::FontType(const std::string &filename, const std::string &fontStyle) :
		m_name(ToFilename(filename, fontStyle)),
		m_texture(Texture::Resource(filename + "/" + fontStyle + ".png")),
		m_metadata(FontMetafile::Resource(filename + "/" + fontStyle + ".fnt"))
	{
	}

	std::string FontType::ToFilename(const std::string &filename, const std::string &fontStyle)
	{
		return "FontType_" + filename + "_" + fontStyle;
	}
}
