#include "FontType.hpp"

namespace acid
{
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
