#include "FontType.hpp"

namespace fl
{
	FontType::FontType(const std::string &textureFile, const std::string &fontFile) :
		m_texture(Texture::Resource(textureFile)),
		m_metadata(FontMetafile::Resource(fontFile))
	{
	}

	FontType::~FontType()
	{
	}
}
