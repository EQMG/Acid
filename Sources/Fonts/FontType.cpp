#include "FontType.hpp"

#include "Resources/Resources.hpp"

namespace acid
{
	std::shared_ptr<FontType> FontType::Resource(const std::string &filename, const std::string &fontStyle)
	{
		auto resource = Resources::Get()->Get(ToFilename(filename, fontStyle));

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
		auto split = FormatString::Split(data, "_");
		std::string filename = split[1];
		std::string fontStyle = split[2];
		return Resource(filename, fontStyle);
	}

	FontType::FontType(const std::string &filename, const std::string &fontStyle) :
		IResource(),
		m_name(ToFilename(filename, fontStyle)),
		m_texture(Texture::Resource(filename + "/" + fontStyle + ".png")),
		m_metadata(FontMetafile::Resource(filename + "/" + fontStyle + ".fnt"))
	{
	}

	FontType::~FontType()
	{
	}

	std::string FontType::ToFilename(const std::string &filename, const std::string &fontStyle)
	{
		return "FontType_" + filename + "_" + fontStyle;
	}
}
